import java.lang.management.*;
import java.util.List;

/**
 * Embfetch — JVM system information display.
 * Loads the native embfetch library and gathers JVM-specific
 * info via ManagementFactory before passing it to the C layer.
 */
public class Embfetch {

    static {
        System.loadLibrary("embfetch");
    }

    // Native methods — implemented in sysinfo_openjdk.c
    private static native void printInfo(
        String version, String implementation, String environment,
        long heapUsed, long heapMax,
        long gcCount, int threadCount
    );

    private static native void printInfoCompiled(
        String version, String implementation, String environment,
        long heapUsed, long heapMax,
        long gcCount, int threadCount
    );

    // Gather JVM info and call native printInfo()
    public static void print() {
        String version        = System.getProperty("java.version");
        String implementation = System.getProperty("java.vm.name", "OpenJDK");
        String vendor         = System.getProperty("java.vendor", "");

        // Detect GraalVM
        String environment;
        if (vendor.contains("GraalVM") || implementation.contains("GraalVM")) {
            environment = "GraalVM";
        } else {
            environment = "OpenJDK";
        }

        // Heap info via MemoryMXBean
        MemoryMXBean memBean = ManagementFactory.getMemoryMXBean();
        MemoryUsage heap     = memBean.getHeapMemoryUsage();
        long heapUsed        = heap.getUsed();
        long heapMax         = heap.getMax();

        // GC collection count via GarbageCollectorMXBeans
        List<GarbageCollectorMXBean> gcBeans = ManagementFactory.getGarbageCollectorMXBeans();
        long gcCount = gcBeans.stream()
                              .mapToLong(GarbageCollectorMXBean::getCollectionCount)
                              .filter(c -> c >= 0)
                              .sum();

        // Thread count via ThreadMXBean
        int threadCount = ManagementFactory.getThreadMXBean().getThreadCount();

        printInfo(version, implementation, environment,
                  heapUsed, heapMax, gcCount, threadCount);
    }

    // Compiled variant — uptime shown as "Compiled"
    public static void printCompiled() {
        String version        = System.getProperty("java.version");
        String implementation = System.getProperty("java.vm.name", "OpenJDK");
        String vendor         = System.getProperty("java.vendor", "");

        String environment = (vendor.contains("GraalVM") ||
                              implementation.contains("GraalVM"))
                           ? "GraalVM" : "OpenJDK";

        MemoryMXBean memBean = ManagementFactory.getMemoryMXBean();
        MemoryUsage heap     = memBean.getHeapMemoryUsage();

        List<GarbageCollectorMXBean> gcBeans = ManagementFactory.getGarbageCollectorMXBeans();
        long gcCount = gcBeans.stream()
                              .mapToLong(GarbageCollectorMXBean::getCollectionCount)
                              .filter(c -> c >= 0)
                              .sum();

        int threadCount = ManagementFactory.getThreadMXBean().getThreadCount();

        printInfoCompiled(version, implementation, environment,
                          heap.getUsed(), heap.getMax(), gcCount, threadCount);
    }

    public static void main(String[] args) {
        print();
    }
}