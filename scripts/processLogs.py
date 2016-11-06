#! /usr/bin/python
"""
Script for processing arduino logs
"""
import argparse
import datetime
import numpy
import os
import sys
import yaml


import matplotlib
# disable matplotlib GUI
# matplotlib.use('Agg')
from matplotlib import pyplot


EXT = "TXT"
DEFAULT_COLOR = "#9FA0A0"


class ParseError(Exception):
    pass


class LogFile(object):
    def __init__(self, name, file_name, full_path, config):
        self.Name = name
        self.FileName = file_name
        self.FullPath = full_path
        self.Config = config

        # Load device mappings
        self.Mappings = parseConfigFile(config['device_config'])['mappings']


class Event(object):
    TIME_FMT = "%H:%M:%S"

    def __init__(self, name, start, end, start_time, end_time, color):
        self.Name = name
        self.Start = start
        self.End = end
        self.StartTime = start_time
        self.EndTime = end_time
        self.Color = color

    @property
    def LengthM(self):
        return (self.EndTime - self.StartTime).total_seconds()/60.0

    @property
    def StartM(self):
        # Hacky bit since logs currently have 0-00-00 dates
        origin = datetime.datetime.strptime("0:00:00", self.TIME_FMT)
        return (self.StartTime-origin).total_seconds()/60.0

    def AddOffset(self, minutes):
        self.StartTime = self.StartTime + datetime.timedelta(minutes=minutes)
        self.EndTime = self.EndTime + datetime.timedelta(minutes=minutes)

    def __repr__(self):
        if self.EndTime:
            delta = self.EndTime - self.StartTime
            return "%s (%ds) - %s" % (self.StartTime.time().isoformat(),
                                      delta.total_seconds(),
                                      self.Name)
        else:
            return "%s - %s" % (self.StartTime.time().isoformat(), self.Name)


def getLogs(log_dir, config):
    logs = []
    for name, log_config in config['log_files'].items():
        file_name = name + config['extension']
        full_path = os.path.join(log_dir, file_name)
        if not os.path.isfile(full_path):
            print "WARNING: Couldn't find log file:", full_path
        else:
            logs.append(LogFile(name, file_name, full_path, log_config))
    return logs


def applyMapping(events, starts, end):
    for event in sorted(events, reverse=True):
        if event.Text in starts:
            if not event.EndTime:
                event.EndTime = end
            return


def parseLine(line):
    ts, text = line.strip().split(' ', 1)
    dt = datetime.datetime.strptime(ts.split('T')[-1], Event.TIME_FMT)
    return text, dt


def matchStartMapping(start, mappings):
    for name, mapping in mappings.items():
        if start == mapping['start']:
            return {"name": name, "matchers": mapping, "color": mapping['color']}
    return {}


def matchEndMapping(start, end, mappings):
    for name, mapping in mappings.items():
        if start == mapping['start'] and end == mapping['end']:
            return {"name": name, "matchers": mapping, "color": mapping['color']}
    return {}


def popEvent(lines, mappings):
    start_text, start_dt = parseLine(lines.pop(0))
    start_mapping = matchStartMapping(start_text, mappings)
    if not start_mapping or len(lines) == 0:
        # print "Single:", start_dt, start_text
        return Event(start_text,
                     start_text,
                     start_text,
                     start_dt,
                     start_dt,
                     color=DEFAULT_COLOR)

    # find the end mapping
    # TODO: this does not allow of interleaved events
    end_text, end_dt = parseLine(lines.pop(0))
    end_mapping = matchEndMapping(start_text, end_text, mappings)
    if not end_mapping:
        return Event(start_text,
                     start_text,
                     start_text,
                     start_dt,
                     start_dt,
                     color=DEFAULT_COLOR)
        # raise ParseError("No end marker for %s / %s " % (start_text, end_text))

    # print "Match:", start_dt, start_text, " == ", end_dt, end_text
    return Event(end_mapping['name'],
                 start_text,
                 end_text,
                 start_dt,
                 end_dt,
                 end_mapping['color'])


def parseLog(log_file):
    events = []
    with open(log_file.FullPath) as f:
        lines = f.readlines()

    while lines:
        try:
            events.append(popEvent(lines, log_file.Mappings))
        except ParseError as e:
            print "WARNING:", str(e)
    return events


def parseLogs(log_files):
    data = {}
    for lf in log_files:
        print "Parsing log file:", lf.FullPath
        data[lf.Name] = parseLog(lf)
        print "Done"

    return data


def parseConfigFile(config_file):
    with open(config_file) as f:
        conf = yaml.load(f)
    return conf


def genGraph(log_data):
    sources = sorted(log_data.keys())
    for y_pos, source in enumerate(sources):
        for event in log_data[source]:
            pyplot.barh(y_pos,
                        event.LengthM,
                        left=event.StartM,
                        color=event.Color,
                        align='center',
                        height=0.3)

    # Handle y axis labels (needs to happen after the data is populated)
    pyplot.yticks(numpy.arange(len(sources)), sources)

    pyplot.show()


def main():
    parser = argparse.ArgumentParser()
    # parser.add_argument("-e", "--ext", default=EXT, help="log extension")
    # parser.add_argument("-d", "--device-config", action="store", help="device config file")
    # parser.add_argument("-c", "--log-config", action="store", help="config file for logs")
    parser.add_argument("config_file", help="config file")
    parser.add_argument("log_dir", help="Directory of log files")
    args = parser.parse_args()

    #
    # Read the global config file
    #
    print "Loading config file:", args.config_file
    config = parseConfigFile(args.config_file)
    print "Done"

    #
    # Get all the log files
    #
    log_files = getLogs(args.log_dir, config)
    if not log_files:
        print "ERROR: No %s log files found" % config['extension']
        sys.exit(1)

    log_data = parseLogs(log_files)

    #
    # Update offsets
    #
    for log, events in log_data.items():
        for event in events:
            event.AddOffset(config['log_files'][log]['offset'])

    #
    # Print and graph the data
    #
    for log, data in log_data.items():
        print log
        for event in data:
            print "    ", event

    genGraph(log_data)

if __name__ == "__main__":
    main()
