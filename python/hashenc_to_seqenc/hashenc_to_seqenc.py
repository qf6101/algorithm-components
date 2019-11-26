import sys
from pathlib import Path
import shutil

if __name__ == '__main__':
    """
    Transform the dataset from hash encoding representation to sequentail encoding
    representation according to the encoding map (the third parameter).
    """
    # input dir of specific month
    inp_dir = Path(sys.argv[1])
    # parent of output dir (as prefix) and the output dir of specific month
    outp_parent_dir = Path(sys.argv[2])
    outp_dir = outp_parent_dir/inp_dir.parts[-1]
    # encoding map from hash encoding to sequantial encoding
    map_dir = sys.argv[3]

    # clean the output dir
    if outp_dir.exists():
        shutil.rmtree(outp_dir)
    outp_dir.mkdir()

    # load encoding map from hash encoding to sequantial encoding
    hash_to_seq = {}
    with open(map_dir, 'r') as f:
        for line in f:
            splits = line.strip().split(' ')
            hash_to_seq[int(splits[0])] = int(splits[1])

    # transform file one by one under input dir
    for p in inp_dir.glob('*'):
        # initial the transformed lines
        outp_lines = []
        with open(p, 'r') as f:
            for line in f:
                # initial the transformed line (the final one and the intermedia one for sorting)
                outp_line = []
                outp_line_ = []
                # parse the line content
                splits = line.strip().split(' ')
                if len(splits[0]) <= 0:
                    continue
                label = int(splits[0])
                feats = splits[1:]
                # transform the label to 1/0 representation (as opposed to +1/-1)
                if label > 0:
                    label = 1
                else:
                    label = 0
                # append the label
                outp_line.append(str(label))
                # for checking hash collision
                duplicated = set()
                # parse features
                for feat in feats:
                    fsplits = feat.split(':')
                    fname = int(fsplits[0])
                    fval = float(fsplits[1])
                    # filter out the too large feature values
                    if (fval < -10) or (fval > 10):
                        continue
                    # replace the hash encode with sequantial encode
                    if fname in  hash_to_seq:
                        new_fname = hash_to_seq[fname]
                        if not new_fname in duplicated:
                            outp_line_.append((new_fname, str(new_fname) + ":" + str(fval)))
                            duplicated.add(new_fname)
                # sort with feature encodes wrt libsvm format
                outp_line_ = sorted(outp_line_, key=lambda x:x[0])
                # append the features
                for l in outp_line_:
                    outp_line.append(l[1])
                # append to the final lines
                outp_lines.append(' '.join(outp_line))
        # write the final lines to output file
        stem_name = str(p.stem)
        print(outp_dir/(stem_name + '.enc'))
        with open(outp_dir/(stem_name + '.enc'), 'w') as f:
            for line in outp_lines:
                f.write(line + '\n')
