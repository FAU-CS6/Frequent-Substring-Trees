###########
# Imports #
###########

import os

############
# Settings #
############

# Path to the the dbtext directory
benchmark_dir = os.path.join(
    "lib", "fsst", "paper", "dbtext"
)  # This path expects an execution from the root directory of the project

# Split the files into categories
machine_readable_identifiers = ["email", "hex", "yago", "uuid", "urls", "urls2", "wiki"]
human_readable_names = [
    "city",
    "credentials",
    "firstname",
    "lastname",
    "street",
    "movies",
]
texts = ["faust", "hamlet", "chinese", "japanese", "wikipedia"]
domain_specific_codes = ["genome", "location"]
tpc_h = ["c_name", "l_comment", "ps_comment"]

####################
# Helper functions #
####################


def get_file_stats(file_path):
    with open(file_path, "r") as f:
        lines = f.readlines()
        num_lines = len(lines)
        avg_line_length = sum([len(line) for line in lines]) / num_lines
        max_line_length = max([len(line) for line in lines])
        unique_lines = len(set(lines))
        return num_lines, avg_line_length, max_line_length, unique_lines


def get_category_stats(file_list):
    for identifier in file_list:
        file_path = os.path.join(benchmark_dir, identifier)
        num_lines, avg_line_length, max_line_length, unique_lines = get_file_stats(
            file_path
        )
        print(f"File: {identifier}")
        print(f"Number of lines: {num_lines}")
        print(f"Average line length: {avg_line_length}")
        print(f"Maximum line length: {max_line_length}")
        print(f"Number of unique lines: {unique_lines}")
        print(f"Percentage of unique lines: {unique_lines / num_lines * 100:.2f}%")
        print()


############
# Main run #
############

if __name__ == "__main__":
    get_category_stats(machine_readable_identifiers)
    get_category_stats(human_readable_names)
    get_category_stats(texts)
    get_category_stats(domain_specific_codes)
    get_category_stats(tpc_h)
