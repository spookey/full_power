#!/usr/bin/env python3


def main(
        batch_size=10,
        indent=' ' * 4,
        input_size=0x00ff,
        int_type='const static uint16_t',
        output_size=0x03ff,
        table_name='lookup_curve',
):
    def _luminance(val):
        val = float((val / input_size) * 100.0)
        if val <= 8:
            val = (val / 902.3)
        else:
            val = ((val + 16) / 116)**3
        return round(val * output_size)

    def _batch(iterable, size=1):
        length = len(iterable)
        for idx in range(0, length, size):
            yield iterable[idx:min(idx + size, length)]

    res = list()
    res.append('// CIE1931 correction table')
    res.append('// Automatically generated')
    res.append('')
    res.append('{int_type} {table_name}[{input_size}] = {{'.format(
        input_size=1 + input_size,
        int_type=int_type,
        table_name=table_name,
    ))
    for bat in _batch([
            _luminance(val) for val in reversed(range(0, 1 + input_size))
    ], size=batch_size):
        res.append('{indent}{line}'.format(
            indent=indent,
            line=' '.join('{val:5},'.format(val=val) for val in bat),
        ))
    res.append('};')
    res.append('')
    return '\n'.join(res)

if __name__ == '__main__':
    print(main())
