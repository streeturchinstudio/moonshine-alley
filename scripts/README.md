# processLogs.py

This script is used to process and visualize log data. To use it, you need a directory of log files and config file that describes how to process each log file.

An example config file might look like:
```
extension: ".log"
log_files:
  Phone1:
    offset: 0
    device_config: "../arduino/phone/logging.yaml"
  Phone2:
    offset: 3
    device_config: "../arduino/phone/logging.yaml"
  Phone3:
    offset: 5
    device_config: "../arduino/phone/logging.yaml"
  StillPhone:
    offset: 15
    device_config: "../arduino/phone/logging.yaml"
```

And an example of a complete command might look like:
```
./processLogs.py myconfig.yaml ../logs-2016-10-24/
```


In the above examples, the `extension` above is combined with each log file name to search for the following logs files:
```
../logs-2016-10-24/Phone1.log
../logs-2016-10-24/Phone2.log
../logs-2016-10-24/Phone3.log
../logs-2016-10-24/StillPhone.log
```

Each log will then be parsed using the device config `../arduino/phone/logging.yaml`. This device config tells the parser how to group log lines into **events**. 

The `offset` adds that many minutes to each event so that you can account for the delay between turning on each device.

