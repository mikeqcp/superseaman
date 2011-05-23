# -*- coding: cp1250 -*-
import os, fnmatch

def process(fileName):
    try:
        file = open(fileName)
    except IOError:
        print 'Błąd odczytu. ( %s )'%(fileName)
        os._exit(1)

    print 'Processing %s...'%(fileName)

    newFile = []
    val = 0
    
    for line in file:
        temp = line.split()
        if (temp[0] == 'f') and len(temp) != 4:
            print line[:(len(line)-1)] + ' in %s deleted.'%(fileName)
            val+=1
        else:
            newFile.append(line)
            
    open(fileName, 'w').writelines(newFile)

    print 'Processing finished successfully with deleting %i lines.\n'%(val)

if __name__ == "__main__":

    for path, subdirs, files in os.walk('.\\'):
        for name in files:
            if fnmatch.fnmatch(name, '*.obj'):
                process(os.path.join(path, name))

