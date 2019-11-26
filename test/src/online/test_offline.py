

import os
import joblib
from sklearn.datasets import load_svmlight_file


def predict(datas, model, hash2seq):
    # print(2, len(datas), datas)
    # import joblib
    clf = joblib.load(model)
    # print(clf)

    # hash2seq
    hash_to_seq = {}
    with open(hash2seq, 'r') as f:
        for line in f:
            splits = line.strip().split(' ')
            hash_to_seq[int(splits[0])] = int(splits[1])
    num_feats = len(hash_to_seq)
    # print(num_feats)

    outp_lines = []
    for data in datas:
        outp_line = []
        outp_line_ = []
        # parse the data content
        splits = data.strip().split(' ')
        # print(splits)
        if len(splits) <= 0:
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
        for l in outp_line_:
            outp_line.append(l[1])
        # append to the final lines
        outp_lines.append(' '.join(outp_line))

    with open('./cache.txt', 'w') as f:
        for line in outp_lines:
            f.write(line + '\n')
    test_data = load_svmlight_file('./cache.txt', n_features=num_feats)
    results = []
    for i in range(test_data[0].shape[0]):
        pred = clf.predict_proba(test_data[0][i])
        # print(pred[0][1])
        results.append("P0:{};P1:{};Y:{}".format(pred[0][0], pred[0][1], test_data[1][i]))
    # print(results)
    return results



