#!/bin/bash
./ms 4 5000 -T -I 4 1 1 1 1 -ej 4.0 2 1 -ej 0.6 3 2 -ej 0.3 4 3 | tail -n +4 | grep -v // > test_ms.txt
