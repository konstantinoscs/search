with open('data.txt') as f:
    with open('dataset.txt', 'w') as data:
        i = 0;
        for line in f:
            if line!='\n':
                new = ''
                new +=str(i) + ' ' + line
                data.write(new)
                i += 1
