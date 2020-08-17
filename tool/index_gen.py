# encoding:utf-8
import os
import sys


class IndexGenerator(object):
    def __init__(self, path):
        self.path = path

    def run(self):
        indexes = []
        with open(self.path, "r", encoding="utf-8") as fs:
            last_unicode = None
            start_unicode = None
            last_length = None
            end_pos = fs.seek(0,2)
            fs.seek(0)
            line = fs.readline()
            while line:
                code, data = line.split(":")
                code = int(code, 16)
                length = len(line)
                if last_unicode is not None:
                    if code - last_unicode != 1 or length != last_length:
                        indexes.append("""{{.start = 0x{:04x},.end = 0x{:04x},.size = {} }}""".format(
                            start_unicode, last_unicode, last_length))
                        start_unicode = code
                    if fs.tell() == end_pos:
                        indexes.append("""{{.start = 0x{:04x},.end = 0x{:04x},.size = {} }}""".format(
                                start_unicode, code, length))
                else:
                    start_unicode = code
                last_unicode = code
                last_length = length
                line = fs.readline()

        with open("{}.index.txt".format(self.path), "w") as fs:
            fs.write(",\n".join(indexes))


if __name__ == "__main__":
    gen = IndexGenerator(os.path.join(os.path.dirname(
        os.path.abspath(__file__)), "unifont-13.0.02.hex"))
    gen.run()
