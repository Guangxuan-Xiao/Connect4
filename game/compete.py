import os
so_dir = "./so/Strategy.so"
testcase_dir = "./TestCases/"
for v2 in reversed(range(2, 102, 2)):
    v1 = "MyAI"
    outfile = "%s_%s.txt" % (v1, v2)
    result = os.path.join("./compete_result/%s_%s.txt" % (v1, v2))
    print(
        "Compete",
        v1,
        v2,
    )
    os.system('./Compete/Compete %s %s "%s" 5 | tee log/%s' %
              (so_dir, testcase_dir + "%d.so" % v2, result, outfile))
