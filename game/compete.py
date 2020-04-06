import os
so_dir = "./so/"
testcase_dir = "./TestCases/"
for so in os.listdir(so_dir):
    for test in os.listdir(testcase_dir):
        v1 = so.split(".")[0]
        v2 = test.split(".")[0]
        outfile = "%s_%s.txt" % (v1, v2)
        result = os.path.join("./compete_result/%s_%s.txt" % (v1, v2))
        print(
            "Compete",
            v1,
            v2,
        )
        os.system('./Compete/Compete %s %s "%s" 1 | tee log/%s' %
                  (so_dir + so, testcase_dir + test, result, outfile))
