# This script run clang-format on all the source files
# in order to share a common and uniform syntax style.

import fnmatch
import os

matches = []
for dir_to_check in ['./ListProto', './ModelSharing', './Scintilla',
 './SideAreaManagement', './TextEditor', './TreeProto']:
    for root, dirnames, filenames in os.walk(dir_to_check):
        for filename in filenames:
            if filename.endswith(('.cpp', '.h', '.hpp')):
                matches.append(os.path.join(root, filename))

print "Files to format: {0}".format(matches)
files = ' '.join(matches)
os.system("C:/LLVM/LLVM_Release_Win64/bin/clang-format.exe -i " + files)
print "Done (press Enter to exit)"
raw_input() # to pause the program for Windows
