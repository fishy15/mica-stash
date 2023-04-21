import subprocess
import pickle

def run_cmd(cmd):
    print(' '.join(cmd))
    subprocess.run(cmd)

def get_protocol_path(protocol):
    if protocol == 'MSI':
        return 'build/X86_MSI/gem5.opt'
    elif protocol == 'MESI':
        return 'build/OurMESI/gem5.opt'
    elif protocol == 'MOSI':
        return 'build/OurMOSI/gem5.opt'
    elif protocol == 'MOESI':
        return 'build/OurMOESI/gem5.opt'
    else:
        raise Exception('not an option: ' + protocol)

def run_test(protocol, test, cpus):
    if test == 'random':
        return run_random(protocol, cpus)

    protocol = get_protocol_path(protocol)
    config = 'configs/mica_stash/simple_ruby.py'
    run_cmd([protocol, config, test, str(cpus)])
    return read_output()

def run_random(protocol, cpus):
    protocol = get_protocol_path(protocol)
    config = 'configs/mica_stash/ruby_test.py'
    run_cmd([protocol, config, '100000', str(cpus)])
    return read_output()

def read_output():
    d = {}
    with open('m5out/stats.txt') as f:
        for line in f:
            ls = line.split()
            if len(ls) > 0:
                if ls[0] == 'simSeconds':
                    d['simSeconds'] = float(ls[1])
                elif ls[0] == 'simTicks':
                    d['simTicks'] = int(ls[1])
                elif ls[0] == 'system.caches.m_latencyHistSeqr::mean':
                    d['latency'] = float(ls[1])
                elif ls[0] == 'system.caches.m_latencyHistSeqr::stdev':
                    d['latency-stddev'] = float(ls[1])
                elif ls[0] == 'system.caches.m_latencyHistSeqr::total':
                    d['latency-count'] = float(ls[1])
                elif ls[0] == 'system.caches.m_hitLatencyHistSeqr::mean':
                    d['hit-latency'] = float(ls[1])
                elif ls[0] == 'system.caches.m_hitLatencyHistSeqr::stdev':
                    d['hit-latency-stdev'] = float(ls[1])
                elif ls[0] == 'system.caches.m_hitLatencyHistSeqr::total':
                    d['hit-count'] = int(ls[1])
                elif ls[0] == 'system.caches.m_missLatencyHistSeqr::mean':
                    d['miss-latency'] = float(ls[1])
                elif ls[0] == 'system.caches.m_missLatencyHistSeqr::stdev':
                    d['miss-latency-stdev'] = float(ls[1])
                elif ls[0] == 'system.caches.m_missLatencyHistSeqr::total':
                    d['miss-count'] = int(ls[1])
                # LD instructions
                elif ls[0] == 'system.caches.RequestType.LD.latency_hist_seqr::mean':
                    d['ld-latency'] = float(ls[1])
                elif ls[0] == 'ld-system.caches.RequestType.LD.latency_hist_seqr::stdev':
                    d['ld-latency-stdev'] = float(ls[1])
                elif ls[0] == 'system.caches.RequestType.LD.latency_hist_seqr::total':
                    d['ld-count'] = int(ls[1])
                elif ls[0] == 'system.caches.RequestType.LD.hit_latency_hist_seqr::mean':
                    d['ld-hit-latency'] = float(ls[1])
                elif ls[0] == 'system.caches.RequestType.LD.hit_latency_hist_seqr::stdev':
                    d['ld-hit-latency-stdev'] = float(ls[1])
                elif ls[0] == 'system.caches.RequestType.LD.hit_latency_hist_seqr::total':
                    d['ld-hit-count'] = int(ls[1])
                elif ls[0] == 'system.caches.RequestType.LD.miss_latency_hist_seqr::mean':
                    d['ld-miss-latency'] = float(ls[1])
                elif ls[0] == 'system.caches.RequestType.LD.miss_latency_hist_seqr::stdev':
                    d['ld-miss-latency-stdev'] = float(ls[1])
                elif ls[0] == 'system.caches.RequestType.LD.miss_latency_hist_seqr::total':
                    d['ld-miss-count'] = int(ls[1])
                # ST instructions
                elif ls[0] == 'system.caches.RequestType.ST.latency_hist_seqr::mean':
                    d['st-latency'] = float(ls[1])
                elif ls[0] == 'system.caches.RequestType.ST.latency_hist_seqr::stdev':
                    d['st-latency-stdev'] = float(ls[1])
                elif ls[0] == 'system.caches.RequestType.ST.latency_hist_seqr::total':
                    d['st-count'] = int(ls[1])
                elif ls[0] == 'system.caches.RequestType.ST.hit_latency_hist_seqr::mean':
                    d['st-hit-latency'] = float(ls[1])
                elif ls[0] == 'system.caches.RequestType.ST.hit_latency_hist_seqr::stdev':
                    d['st-hit-latency-stdev'] = float(ls[1])
                elif ls[0] == 'system.caches.RequestType.ST.hit_latency_hist_seqr::total':
                    d['st-hit-count'] = int(ls[1])
                elif ls[0] == 'system.caches.RequestType.ST.miss_latency_hist_seqr::mean':
                    d['st-miss-latency'] = float(ls[1])
                elif ls[0] == 'system.caches.RequestType.ST.miss_latency_hist_seqr::stdev':
                    d['st-miss-latency-stdev'] = float(ls[1])
                elif ls[0] == 'system.caches.RequestType.ST.miss_latency_hist_seqr::total':
                    d['st-miss-latency-count'] = float(ls[1])

    return d

def run_per_cpus(protocol, test):
    d = {}
    cpus = [1, 2, 4, 8, 16, 32]
    for c in cpus:
        d[c] = run_test(protocol, test, c)
    return d

def run_tests(protocol):
    tests = [
            'sparseadd64', 'blockadd64', 'roundrobin64', 'prefixscan64',
            'sparseadd100', 'blockadd100', 'roundrobin100', 'prefixscan100',
            'sparseadd500', 'blockadd500', 'roundrobin500', 'prefixscan500',
            'sparseadd1000', 'blockadd1000', 'roundrobin1000', 'prefixscan1000',
            'random'
    ]

    tests = list(filter(lambda x: 'roundrobin' in x, tests))

    d = {}
    for t in tests:
        d[t] = run_per_cpus(protocol, t)
    return d

def main():
    d = {}
    protocols = ['MSI', 'MESI', 'MOSI', 'MOESI']
    for p in protocols:
        d[p] = run_tests(p)

    with open('data_rr.bin', 'wb') as f:
        pickle.dump(d, f)

if __name__ == '__main__':
    main()
