#!/usr/bin/env python3

from argparse import ArgumentParser
from os import path

SOURCE = path.dirname(path.realpath(__file__))
DEFINE = '_INCL_STRING'


def parse():
    parser = ArgumentParser('conv')
    parser.add_argument('files', nargs='+', help='input files')
    parser.add_argument(
        '-s', '--source', default=SOURCE,
        action='store', help='source directory (default: "%(default)s")'
    )
    parser.add_argument(
        '-t', '--target', default=path.dirname(SOURCE),
        action='store', help='target directory (default: "%(default)s")'
    )
    parser.add_argument(
        '-f', '--force', default=False,
        action='store_true', help='force overwrite (default: "%(default)s")'
    )
    parser.add_argument(
        '-d', '--define', default=DEFINE,
        action='store', help='define symbol (default: "%(default)s")'
    )

    return parser.parse_args()


def read_file(filename):
    if path.exists(filename):
        with open(filename, 'r') as handle:
            return handle.read()


def clamp(define, content):
    return '\n'.join(['{}('.format(define), content.strip(), ')'])


def write_file(filename, content):
    with open(filename, 'w') as handle:
        return handle.write(content)


def main():
    args = parse()
    for item in [path.basename(fl) for fl in args.files]:
        target = path.join(args.target, item)
        if path.exists(target) and not args.force:
            print('skipping', item, 'already present in', args.target)
            continue

        content = read_file(path.join(args.source, item))
        if not content:
            print('file', item, 'not present in', args.source)
            return False

        if not write_file(target, clamp(args.define, content)):
            print('could not write', item, 'in', args.target)
            return False
    return True


if __name__ == '__main__':
    exit(not main())
