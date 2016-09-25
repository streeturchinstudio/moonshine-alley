#! /usr/bin/python
"""
Script for processing arduino logs
"""
import argparse
import datetime
import os
import sys

EXT = "TXT"


class Event(object):
    TIME_FMT = "%H:%M:%S"

    def __init__(self, text, start, end=None):
        self.Text = text
        self.StartTime = start
        self.EndTime = end

    def __repr__(self):
        if self.EndTime:
            delta = self.EndTime - self.StartTime
            return "%s (%ds) - %s" % (self.StartTime.time().isoformat(),
                                      delta.total_seconds(),
                                      self.Text)
        else:
            return "%s - %s" % (self.StartTime.time().isoformat(), self.Text)


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


def parseLog(log_file, mappings):
    events = []
    with open(log_file) as f:
        lines = f.readlines()

    for line in lines:
        ts, text = line.strip().split(' ', 1)
        dt = datetime.datetime.strptime(ts.split('T')[-1], Event.TIME_FMT)
        if text in mappings:
            applyMapping(events, mappings[text], dt)
        else:
            events.append(Event(text, dt))
    return events


def parseLogs(log_files, mappings):
    return [(lf, parseLog(lf, mappings)) for lf in log_files]


def parseMappings(mappings):
    maps = {}
    print mappings
    for mapping in mappings:
        start, end = mapping.split(':', 1)
        maps.setdefault(end, [])
        maps[end].append(start)
    print maps
    return maps


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-e", "--ext", default=EXT, help="log extension")
    parser.add_argument("-m", "--mapping", action="append", default=[], help="mappings of event start and ends")
    parser.add_argument("logdir", help="Directory of log files")
    args = parser.parse_args()

    mappings = parseMappings(args.mapping)

    log_files = getLogs(args.logdir, args.ext)
    log_data = parseLogs(log_files, mappings)
    for log, data in log_data:
        print log
        for event in data:
            print "    ", event


if __name__ == "__main__":
    main()
