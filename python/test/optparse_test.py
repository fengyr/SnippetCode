#!/usr/bin/env python
#!encoding=utf8

from optparse import OptionParser


def main():
    usage = "usage: %prog [options] arg"
    parser = OptionParser(usage)
    parser.add_option("-f", "--file", action="store", type="string",
                      dest="filename", help="read data from FILENAME")
    parser.add_option("-v", "--verbose", action="store_true", dest="verbose")
    parser.add_option("-q", "--quiet", action="store_false", dest="verbose")

    (options, args) = parser.parse_args()
#	if len(args) != 1:
#		parser.error("incorrect number of arguments")
    print len(args)
    if options.verbose:
        print "reading %s..." % options.filename
    if options.filename:
        print 'open %s' % options.filename


if __name__ == "__main__":
    main()
