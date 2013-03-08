#!/usr/bin/python
#!encoding=utf8

import convert
import picmark
import config
import sys
import os
from optparse import OptionParser

#CODEC_FORMAT = {文件格式:(编码器, 封装格式)}
CODEC_FORMAT = {'jpeg': ('mpeg4', '.avi'), 'ppm': ('ppm', '.avi'),
                'bmp': ('copy', '.avi'), 'png': ('mjpeg', '.avi'),
                'tiff': ('ppm', '.avi'),  # for dump YUV
                }


def ParseArgs():
    usage = 'usage: %prog [options] -i input file [-o output file]'
    parser = OptionParser(usage=usage)

    format_help = '\n'
    for key, val in CODEC_FORMAT.items():
        format_help = format_help + '%s, ' % key
    operate_help = '\n'
    operate_help = operate_help + 'AddTime, ' + 'BlackWhite, ' + 'DumpYUV'

    parser.add_option('-i', '--input', dest='input_file',
                      type='string', help='source video file')
    parser.add_option(
        '-o', '--output', dest='output_file', type='string', help='destiny video file',
        default=config.OUTPUT)
    parser.add_option('-t', '--time', dest='duration', type='int',
                      help='restrict the duration in seconds, default is 20 sec', default=config.DURATION)
    parser.add_option('-s', '--scale', dest='scale', type='string',
                      help='convert video scale, example: 352x288', default=config.SCALE)
    parser.add_option('-f', '--format', dest='format', type='string',
                      help=format_help, default=config.FORMAT)
    parser.add_option('--author', dest='author', type='string',
                      help='the author name', default=config.AUTHOR)
    parser.add_option('-r', '--rate', dest='rate', type='int',
                      help='video fps', default=config.FPS)
    parser.add_option('--operate', dest='action', type='string',
                      help=operate_help, default=config.ACTION)
    (options, args) = parser.parse_args()
    if not options.input_file:
        parser.print_help()
        sys.exit(1)
    return (options, args)


def video2pic():
    convert.video2pic(
        options.input_file, tempfiles, options.duration, options.rate)


def do_pic():
    marker = picmark.Marker(config.DIR, options.format, options.action,
                            Scale=options.scale, Fps=options.rate)
    if	marker.DoWork() != 'Finished':
        clear_up()
        sys.exit(1)

    if options.scale == config.SCALE:
        scale = marker.GetPicSize()
        if scale is None:
            options.scale = config.SCALE
        else:
            options.scale = scale


def pic2video():
    try:
        CODEC_FORMAT[options.format]
    except:
        print 'Picture format is not validate'
        return
    convert.pic2video(
        tempfiles, options.output_file, CODEC_FORMAT[options.format][0],
        CODEC_FORMAT[options.format][1], options.scale, options.rate, options.author)


def videodump():
    if options.scale == config.SCALE:
        options.scale = 'vga'
    convert.videodump(options.input_file, options.output_file,
                      options.scale, options.duration)


def clear_up():
    os.system('rm -rf %s' % config.DIR)

(options, args) = ParseArgs()
tempfiles = config.DIR + '/' + config.FILES + '.' + options.format
#如果是Dump,视频为YUV格式
if options.action == 'DumpYUV':
    videodump()
else:
    #将源视频转换为图片
    video2pic()
    #图片操作
    do_pic()
    #将图片转换成视频
    pic2video()
    clear_up()
