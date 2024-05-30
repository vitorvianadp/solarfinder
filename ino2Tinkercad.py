from os import replace
import re
import argparse

LINE_WIDTH = 70


def commentIncludes(lines):
    commented_code = []
    for line in lines:
        commented_code.append(line.replace('#include', '// #include'))
        # commented_code.append(line)
    return commented_code


def fileSeparator(file_name):
    return [f'// {"="*LINE_WIDTH}\n', f'// {file_name}\n\n']


def readComponentSingleFile(name, extension):
    code = []
    file_name = name + extension
    try:
        with open(file_name) as f:
            code += fileSeparator(file_name)
            code += commentIncludes(f.readlines())
            code.append('\n\n')
    except IOError:
        print(f'Warning: {file_name} does not exist')
    return code


def getInterfaceName(file_name):
    code = []
    with open(file_name) as f:
        include_searches = [
            re.search('#include\s+"([\w]+)', line) for line in f.readlines()
            if '#include' in line
        ]
        if not include_searches:
            return None
        return include_searches[0].group(1)


def readComponentFiles(name):
    source_code = []

    # Check component header for interface header
    if interface_name := getInterfaceName(f'{name}.h'):
        source_code += readComponentSingleFile(interface_name, '.h')

    # Add header file contents
    source_code += readComponentSingleFile(name, '.h')

    # Add cpp file contents
    source_code += readComponentSingleFile(name, '.cpp')

    return source_code


def convert2Tinkercad(input_file, output_file):
    # Read original file
    originalfile = []
    with open(input_file) as f:
        originalfile = f.readlines()

    # Create new file substituting include directives with actual file contents
    main_separator_added = False
    init_includes = False
    tinkerfile = []
    for line in originalfile:
        if include_search := re.search('#include\s+"([\w]+)', line,
                                       re.IGNORECASE):
            component_name = include_search.group(1)
            tinkerfile += readComponentFiles(component_name)
            init_includes = True
        else:
            if (not main_separator_added and init_includes):
                tinkerfile += fileSeparator(input_file)
                main_separator_added = True
            tinkerfile.append(line)

    # Save result to file
    with open(output_file, 'w') as output:
        for line in tinkerfile:
            output.write(line)


if __name__ == '__main__':
    # Parse command line arguments
    parser = argparse.ArgumentParser(
        description='Convert alarm Arduino project to Tinkercad')
    parser.add_argument('input_file',
                        nargs='?',
                        default='alarm.ino',
                        help='Input Arduino .ino file')
    parser.add_argument('-o', '--outfile', help='output file')
    args = parser.parse_args()
    input_file = args.input_file
    output_file = args.outfile if args.outfile is not None else args.input_file.replace(
        '.ino', '_tinkercad.ino')

    convert2Tinkercad(input_file, output_file)