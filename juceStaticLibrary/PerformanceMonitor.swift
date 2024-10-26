import SwiftUI
import Darwin

class PerformanceMonitor: ObservableObject {
    @Published var cpuUsage: Double = 0
    @Published var memoryUsage: UInt64 = 0
    private var timer: Timer?
    
    init() {
        timer = Timer.scheduledTimer(withTimeInterval: 1.0, repeats: true) { [weak self] _ in
            self?.updateMetrics()
        }
    }
    
    private func updateMetrics() {
        cpuUsage = getHostCPUUsage()
        memoryUsage = getMemoryUsage()
    }
    
    private func getHostCPUUsage() -> Double {
        var cpuInfo = processor_info_array_t(bitPattern: 0)
        var numCpuInfo: mach_msg_type_number_t = 0
        var numProcessors: natural_t = 0
        
        let result = host_processor_info(mach_host_self(),
                                       PROCESSOR_CPU_LOAD_INFO,
                                       &numProcessors,
                                       &cpuInfo,
                                       &numCpuInfo)
        
        guard result == KERN_SUCCESS else {
            return -1.0
        }
        
        defer {
            if let cpuInfo = cpuInfo {
                vm_deallocate(mach_task_self_,
                            vm_address_t(UInt(bitPattern: cpuInfo)),
                            vm_size_t(numCpuInfo * UInt32(MemoryLayout<integer_t>.stride)))
            }
        }
        
        var totalUsage: Double = 0
        
        if let cpuInfo = cpuInfo {
            for i in 0..<Int(numProcessors) {
                let offset = Int32(CPU_STATE_MAX) * Int32(i)
                let user = Int32(cpuInfo[Int(offset + Int32(CPU_STATE_USER))])
                let system = Int32(cpuInfo[Int(offset + Int32(CPU_STATE_SYSTEM))])
                let idle = Int32(cpuInfo[Int(offset + Int32(CPU_STATE_IDLE))])
                
                let inUse = Double(user + system)
                let total = Double(user + system + idle)
                
                totalUsage += (inUse / total) * 100.0
            }
        }
        
        return totalUsage / Double(numProcessors)
    }
    
    private func getMemoryUsage() -> UInt64 {
        var info = mach_task_basic_info()
        var count = mach_msg_type_number_t(MemoryLayout<mach_task_basic_info>.size)/4
        
        let kerr: kern_return_t = withUnsafeMutablePointer(to: &info) {
            $0.withMemoryRebound(to: integer_t.self, capacity: Int(count)) {
                task_info(mach_task_self_,
                         task_flavor_t(MACH_TASK_BASIC_INFO),
                         $0,
                         &count)
            }
        }
        
        if kerr == KERN_SUCCESS {
            return info.resident_size
        }
        return 0
    }
    
    deinit {
        timer?.invalidate()
    }
}

struct PerformanceView: View {
    @StateObject private var monitor = PerformanceMonitor()
    
    var body: some View {
        VStack(alignment: .leading, spacing: 4) {
            HStack {
                Image(systemName: "cpu")
                    .imageScale(.small)
                Text("CPU: \(String(format: "%.1f", monitor.cpuUsage))%")
                    .font(.system(.caption, design: .monospaced))
            }
            
            HStack {
                Image(systemName: "memorychip")
                    .imageScale(.small)
                Text("RAM: \(formatMemory(monitor.memoryUsage))")
                    .font(.system(.caption, design: .monospaced))
            }
        }
        .padding(8)
        .background(Color.black.opacity(0.7))
        .foregroundColor(.white)
        .cornerRadius(8)
    }
    
    private func formatMemory(_ bytes: UInt64) -> String {
        let mb = Double(bytes) / 1024 / 1024
        return String(format: "%.1f MB", mb)
    }
}
