#!/usr/bin/env python3

from argparse import ArgumentParser
from os import makedirs, path
from sys import stderr, stdout

PARENT = path.dirname(path.realpath(__file__))
CRUMBS = path.join(PARENT, 'lib/serve')
DEFINE = '_INCL_STRING'
PRESET = ['basic.html', 'funct.js', 'plugs.html', 'style.css']


def parse():
    parser = ArgumentParser('conv')
    parser.add_argument(
        'files', nargs='*', default=PRESET,
        help='input files (default: "%(default)s")'
    )
    parser.add_argument(
        '-s', '--source', default=path.join(CRUMBS, 'raw'),
        action='store', help='source directory (default: "%(default)s")'
    )
    parser.add_argument(
        '-t', '--target', default=path.join(CRUMBS, 'etc'),
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


def msg(*txt, res=False):
    out = (stdout if res else stderr)
    out.write('' if res else 'error: ')
    out.write(' '.join(str(t) for t in txt))
    out.write('\n')
    out.flush()
    return res


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
    if not path.exists(args.target):
        makedirs(args.target)
        msg('created target path', args.target)

    for item in [path.basename(fl) for fl in args.files]:
        target = path.join(args.target, '_{}'.format(item))
        if not args.force and path.exists(target):
            msg('skipping', item, 'already present in', args.target)
            continue

        content = read_file(path.join(args.source, item))
        if not content:
            return msg('file', item, 'not present in', args.source)

        if not write_file(target, clamp(args.define, content)):
            return msg('could not write', item, 'in', args.target)
        msg(item, 'done', res=True)

    return msg('end!', res=True)


if __name__ == '__main__':
    exit(not main())
