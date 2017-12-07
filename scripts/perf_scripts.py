from subprocess import Popen, PIPE

def one_experiment(N, exe_path):
    exe = exe_path
    comand = exe + str(N) +" " + str(1)
    proc = Popen(comand, shell=True, stdout=PIPE, stderr=PIPE)
    proc.wait()
    res = proc.communicate()
    if proc.returncode:
       return res[1]
    return res[0]

def serial_experiment(N, count, exe_path):
    avg_time = 0
    for i in range(count):
      time = float(one_experiment(N, exe_path))
      avg_time += time

    avg_time /= count
    return avg_time

def write_log(step, exe_path, save_path):
    lower_bound = 1
    upper_bound = 1000000
    i = lower_bound
    f = open(save_path, 'w')
    while(i <= upper_bound):
        avg_time = float(serial_experiment(i, 10, exe_path))
        f.write(str(avg_time) + "\n")
        i += step

    f.close()
write_log(10000, '../cmake-build-debug/sample/bytes_radix_sort ', 'log/log_bytes_radix_sort.txt');
write_log(10000, '../cmake-build-debug/sample/compact_bytes_radix_sort ', 'log/log_compact_bytes_radix_sort.txt');
write_log(10000, '../cmake-build-debug/sample/lsd_compact_radix_sort ', 'log/log_lsd_compact_radix_sort.txt');
write_log(10000, '../cmake-build-debug/sample/lsd_radix_sort ', 'log/log_lsd_radix_sort.txt');
