#! /usr/bin/python
"""
Script for processing arduino logs
"""
import argparse
import datetime
import os
import sys
import yaml


import matplotlib
# disable matplotlib GUI
matplotlib.use('Agg')
from matplotlib import pyplot


EXT = "TXT"


class ParseError(Exception):
    pass


class Event(object):
    TIME_FMT = "%H:%M:%S"

    def __init__(self, name, start, end, start_time, end_time):
        self.Name = name
        self.Start = start
        self.End = end
        self.StartTime = start_time
        self.EndTime = end_time

    def __repr__(self):
        if self.EndTime:
            delta = self.EndTime - self.StartTime
            return "%s (%ds) - %s" % (self.StartTime.time().isoformat(),
                                      delta.total_seconds(),
                                      self.Name)
        else:
            return "%s - %s" % (self.StartTime.time().isoformat(), self.Name)


def getLogs(directory, ext):
    logs = []
    for f in sorted(os.listdir(directory)):
        if f.endswith("." + ext):
            logs.append(os.path.join(directory, f))
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
            return {"name": name, "matchers": mapping}
    return {}


def matchEndMapping(start, end, mappings):
    for name, mapping in mappings.items():
        if start == mapping['start'] and end == mapping['end']:
            return {"name": name, "matchers": mapping}
    return {}


def popEvent(lines, mappings):
    start_text, start_dt = parseLine(lines.pop(0))
    start_mapping = matchStartMapping(start_text, mappings)
    if not start_mapping or len(lines) == 0:
        # print "Single:", start_dt, start_text
        return Event(start_text, start_text, start_text, start_dt, start_dt)

    # find the end mapping
    # TODO: this does not allow of interleaved events
    end_text, end_dt = parseLine(lines.pop(0))
    end_mapping = matchEndMapping(start_text, end_text, mappings)
    if not end_mapping:
        raise ParseError("No end marker for %s / %s " % (start_text, end_text))

    # print "Match:", start_dt, start_text, " == ", end_dt, end_text
    return Event(end_mapping['name'],
                 start_text,
                 end_text,
                 start_dt,
                 end_dt)


def parseLog(log_file, mappings):
    events = []
    with open(log_file) as f:
        lines = f.readlines()

    while lines:
        events.append(popEvent(lines, mappings))
    return events


def parseLogs(log_files, mappings):
    return [(lf, parseLog(lf, mappings)) for lf in log_files]


def parseConfigFile(config_file):
    with open(config_file) as f:
        conf = yaml.load(f)
    return conf


def genGraph(log_data):
    # use pyplot.hlines(), relative times, color config?
    pass


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-e", "--ext", default=EXT, help="log extension")
    parser.add_argument("-c", "--config", action="store", help="logging config file")
    parser.add_argument("-f", "--filter", type=int, help="filter events less than N seconds")
    parser.add_argument("-o", "--offset", type=int, help="Offset time by N seconds")
    parser.add_argument("logdir", help="Directory of log files")
    args = parser.parse_args()

    if args.config:
        config = parseConfigFile(args.config)
    else:
        config = {"mappings": {}}

    log_files = getLogs(args.logdir, args.ext)
    log_data = parseLogs(log_files, config['mappings'])

    for log, data in log_data:
        row = [log]
        for event in data:
            # row.append
            print "    ", event


if __name__ == "__main__":
    main()
