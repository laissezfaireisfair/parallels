# Lab 2.1 report

## Machines
### CPU
|              | Local                              | Remote                                   |
|--------------|------------------------------------|------------------------------------------|
| Model name   | AMD Ryzen 7 5800X 8-Core Processor | Intel(R) Xeon(R) Gold 6248 CPU @ 2.50GHz |
| CPU(s)       | 16                                 | 80                                       |
| CPU max MHz  | 4850.1948                          | 3900.0000                                |
| CPU min MHz  | 2200.0000                          | 1000.0000                                |
| L1d cache    | 256 KiB                            | 1.3 MiB                                  |
| L1i cache    | 256 KiB                            | 1.3 MiB                                  |
| L2 cache     | 4 MiB                              | 40 MiB                                   |
| L3 cache     | 32 MiB                             | 55 Mib                                   |
| NUMA node(s) | 1                                  | 2                                        |

### Server name
| Local   | Remote                |
|---------|-----------------------|
| MS-7C56 | ProLiant XL270d Gen10 |

### Numa nodes memory
| Local node 0 | Remote node 0 | Remote node 1 |
|--------------|---------------|---------------|
| 15915 MB     | 385636 MB     | 387008 MB     |

### OS
|             | Local      | Remote              |
|-------------|------------|---------------------|
| Pretty name | Arch Linux | Ubuntu 22.04.03 LTS |
| id          | arch       | ubuntu              |

## Performance

### 40 000 elements
| Threads | Time local | Time remote |
|---------|------------|-------------|
| 1       | 41.4s      | 5m18s       |
| 2       | 20.9s      | 2m36s       |
| 4       | 12.5s      | 1m23s       |
| 7       | 9.0s       | 55s         |
| 8       | 8.34s      | 51s         |
| 16      | 8.11s      | 32s         |
| 20      | 8.18s      | 29s         |
| 40      | 8.14s      | 19s         |

### 20 000 elements
| Threads | Time local | Time remote |
|---------|------------|-------------|
| 1       | 8.9s       | 36.7s       |
| 2       | 4.7s       | 21.9s       |
| 4       | 2.7s       | 12.3s       |
| 7       | 1.8s       | 8.7s        |
| 8       | 1.63s      | 7.7s        |
| 16      | 1.58s      | 6.1s        |
| 20      | 1.63s      | 4.8s        |
| 40      | 1.57s      | 4.0s        |