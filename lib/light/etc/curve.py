#!/usr/bin/env python3


def main(
        batch_size=10,
        indent=' ' * 4,
        input_size=1024,
        int_type='const static uint16_t',
        output_size=1024,
        table_name='curve',
):
    def _luminance(val):
        val = float(val * 100.0)
        if val <= 8:
            return val / 902.3
        return ((val + 16) / 116)**3

    def _batch(iterable, size=1):
        length = len(iterable)
        for idx in range(0, length, size):
            yield iterable[idx:min(idx + size, length)]

    numbers = [
        round(_luminance(val / input_size) * output_size)
        for val in range(0, 1 + input_size)
    ]

    res = list()
    res.append('// CIE1931 correction table')
    res.append('// Automatically generated')
    res.append('')
    res.append('{int_type} {table_name}[{input_size}] = {{'.format(
        input_size=1 + input_size,
        int_type=int_type,
        table_name=table_name,
    ))
    for bat in _batch(numbers, size=batch_size):
        res.append('{indent}{line}'.format(
            indent=indent,
            line=' '.join('{val:5},'.format(val=val) for val in bat),
        ))
    res.append('};')
    res.append('')
    return '\n'.join(res)

if __name__ == '__main__':
    print(main())
