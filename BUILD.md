Build Instructions:
# Let `n` be the number of cores you want to build on +1 (eg n = 9 if building on 8 cores)
For MSI, we run `scons build/X86_MSI/gem5.opt --default=X86 PROTOCOL=MSI -j n`.
For each of our other protocols P from {OurMESI, OurMOSI, OurMOESI},
we run `scons build/P/gem5.opt default=X86 PROTOCOL=P -j n`

This creates the executable in the specified directory (`build/P/`).

Run Instructions:
To actually run a benchmark on the simulator with the specified protocol, we run `build/P/gem5.opt configs/mica_stash/simple_ruby.py <benchmark>` where benchmark is any of the executable files found within `tests/test-progs/benchmarks/tests/build`. The executables should be of the form: `<benchmark><size>` e.g. blockadd100. If the executables are not present in this folder, you can go to `tests/test-progs/benchmarks/tests/` and run `make all`.

Viewing Statistics:
After running a specific executable, you can view the metrics regarding its run in `m5out/stats.txt`

List of used benchmarks:
blockadd100   blockadd500  prefixscan     prefixscan1000  prefixscan64   sparseadd100   sparseadd500  vectoradd blockadd1000  blockadd64   prefixscan100  prefixscan500 sparseadd1000  sparseadd64
