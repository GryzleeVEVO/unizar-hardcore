import csv

# paths_to_files = [
#     "ouptut_2.csv",
#     "ouptut_3.csv",
#     "ouptut_4.csv",
#     "ouptut_5.csv",
#     "ouptut_6.csv",
#     "ouptut_7.csv",
#     "ouptut_8.csv",
#     "ouptut_9.csv",
#     "ouptut_10.csv",
#     "ouptut_11.csv",
#     "ouptut_12.csv",
#     "ouptut_13.csv",
#     "ouptut_14.csv",
#     "ouptut_15.csv",
#     "ouptut_16.csv",
#     "ouptut_17.csv",
#     "ouptut_18.csv",
#     "ouptut_19.csv",
#     "ouptut_20.csv",
# ]


paths_to_files = [
    "out_0.1.csv",
    "out_0.2.csv",
    "out_0.3.csv",
    "out_0.4.csv",
    "out_0.5.csv",
    "out_0.6.csv",
    "out_0.7.csv",
    "out_0.8.csv",
    "out_0.9.csv",
]


def dolor(path):
    with open(path) as f:
        next(f)
        csv_reader = csv.reader(f, delimiter=";")

        mem = 0
        time_ms = 0
        time_total = 0

        for row in csv_reader:
            mem += float(row[0])
            time_ms += float(row[1])
            time_total += float(row[2])

        print(mem / 10, time_ms / 10, time_total / 10)


for path in paths_to_files:
    dolor(path)
