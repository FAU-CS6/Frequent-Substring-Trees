###########
# Imports #
###########

import pandas as pd
import numpy as np
import os
import statistics
import warnings

warnings.simplefilter(action="ignore", category=FutureWarning)

############
# Settings #
############

# Path to the benchmark results (CSV file)
input_csv = os.path.join(
    "benchmark", "running", "results.csv"
)  # This path expects an execution from the root directory of the project

# Path to the output folder
output_dir = os.path.join(
    "benchmark", "running", "preprocessed"
)  # This path expects an execution from the root directory of the project

# Path to the compressed files (directory)
compressed_files_dir = os.path.join(
    "benchmark", "running", "compressed_files"
)  # This path expects an execution from the root directory of the project

# Path to the files that have been compressed during the benchmark (directory)
benchmark_dir = os.path.join(
    "lib", "fsst", "paper", "dbtext"
)  # This path expects an execution from the root directory of the project

####################
# Helper functions #
####################


def single_comparison_to_fsst(value_to_compare: pd.Series, fsst_value: pd.Series):
    return int(value_to_compare / fsst_value * 100)


def compare_values_to_fsst(values_to_compare: pd.DataFrame, fsst_values: pd.DataFrame):
    return_df = values_to_compare.copy()

    return_df["duration_compared_to_fsst"] = single_comparison_to_fsst(
        values_to_compare["duration"], fsst_values["duration"]
    )
    return_df["duration_symbol_table_compared_to_fsst"] = single_comparison_to_fsst(
        values_to_compare["duration_symbol_table"], fsst_values["duration_symbol_table"]
    )
    return_df["duration_compress_no_symbol_compared_to_fsst"] = (
        single_comparison_to_fsst(
            values_to_compare["duration_compress_no_symbol"],
            fsst_values["duration_compress_no_symbol"],
        )
    )
    return_df["compression_ratio_compared_to_fsst"] = single_comparison_to_fsst(
        values_to_compare["compression_ratio"], fsst_values["compression_ratio"]
    )
    return_df["compression_factor_compared_to_fsst"] = single_comparison_to_fsst(
        values_to_compare["compression_factor"], fsst_values["compression_factor"]
    )
    return_df["compression_speed_in_mb_per_s_compared_to_fsst"] = (
        single_comparison_to_fsst(
            values_to_compare["compression_speed_in_mb_per_s"],
            fsst_values["compression_speed_in_mb_per_s"],
        )
    )
    return_df["compression_speed_in_mb_per_s_without_symbol_table_compared_to_fsst"] = (
        single_comparison_to_fsst(
            values_to_compare["compression_speed_in_mb_per_s_without_symbol_table"],
            fsst_values["compression_speed_in_mb_per_s_without_symbol_table"],
        )
    )

    return return_df


def calculate_compression_factor(df: pd.DataFrame):
    result_df = df.copy()

    result_df["compression_factor"] = round(
        1 / (result_df["compression_ratio"] / 100), 2
    )

    return result_df


def calculate_compression_speed_in_mb_per_s(df: pd.DataFrame):
    result_df = df.copy()

    # Get the size of the input file in MB
    size_input_file = (
        os.path.getsize(os.path.join(benchmark_dir, result_df.index[0])) / 1024 / 1024
    )

    # Calculate the compression speed in MB/s
    result_df["compression_speed_in_mb_per_s"] = round(
        size_input_file / (result_df["duration"] / 1000), 2
    )
    result_df["compression_speed_in_mb_per_s_without_symbol_table"] = round(
        size_input_file / (result_df["duration_compress_no_symbol"] / 1000), 2
    )

    return result_df


def get_preprocessed_df(input_df: pd.DataFrame, input_file: str):
    # Copy the input dataframe
    df = input_df.copy()

    # Create a result dataframe
    result_df = pd.DataFrame(
        {
            "input_file": [input_file],
        }
    )

    # Filter out the tuples of fsst_without_fst
    fsst_without_fst = (
        df[(df["name"] == "fsst_without_fst") & (df["input_file"] == input_file)]
        .groupby(["input_file"])
        .agg(
            {
                "duration": "mean",
                "duration_symbol_table": "mean",
                "duration_compress_no_symbol": "mean",
                "compression_ratio": "mean",
            }
        )
    )

    # Calculate the compression factor
    fsst_without_fst = calculate_compression_factor(fsst_without_fst)

    # Calculate the compression speed in MB/s
    fsst_without_fst = calculate_compression_speed_in_mb_per_s(fsst_without_fst)

    # We want to add columns comparing the values of fsst_without_fst to fsst_with_fst_and_fsst_sampling
    fsst_without_fst = compare_values_to_fsst(fsst_without_fst, fsst_without_fst)

    # Rename the columns to add a suffix
    suffix = "fsst_without_fst"
    fsst_without_fst_temp = fsst_without_fst.rename(
        columns={
            col: f"{col}_{suffix}"
            for col in fsst_without_fst.columns
            if col != "input_file"
        }
    )

    # Merge the values of fsst_without_fst to the result_df
    result_df = pd.merge(result_df, fsst_without_fst_temp, on="input_file", how="outer")

    # Filter out the tuples of fsst_with_fst_and_fsst_sampling
    fsst_with_fst_and_fsst_sampling = (
        df[
            (df["name"] == "fsst_with_fst_and_fsst_sampling")
            & (df["input_file"] == input_file)
        ]
        .groupby(["input_file"])
        .agg(
            {
                "duration": "mean",
                "duration_symbol_table": "mean",
                "duration_compress_no_symbol": "mean",
                "compression_ratio": "mean",
            }
        )
    )

    # Calculate the compression factor
    fsst_with_fst_and_fsst_sampling = calculate_compression_factor(
        fsst_with_fst_and_fsst_sampling
    )

    # Calculate the compression speed in MB/s
    fsst_with_fst_and_fsst_sampling = calculate_compression_speed_in_mb_per_s(
        fsst_with_fst_and_fsst_sampling
    )

    # We want to add columns comparing the values of fsst_with_fst_and_fsst_sampling to fsst_without_fst
    fsst_with_fst_and_fsst_sampling = compare_values_to_fsst(
        fsst_with_fst_and_fsst_sampling, fsst_without_fst
    )

    # Rename the columns to add a suffix
    suffix = "fsst_with_fst_and_fsst_sampling"
    fsst_with_fst_and_fsst_sampling_temp = fsst_with_fst_and_fsst_sampling.rename(
        columns={
            col: f"{col}_{suffix}"
            for col in fsst_with_fst_and_fsst_sampling.columns
            if col != "input_file"
        }
    )

    # Merge the values of fsst_with_fst_and_fsst_sampling to the result_df
    result_df = pd.merge(
        result_df, fsst_with_fst_and_fsst_sampling_temp, on="input_file", how="outer"
    )

    # Get the tuples of the input file that are measured while using fst in fsst
    fsst_with_fst_df = df[
        (df["input_file"] == input_file) & (df["name"] == "fsst_with_fst")
    ]

    # Calculate the average values for duration and compression_ratio
    fsst_with_fst_df = (
        fsst_with_fst_df.groupby(["input_file", "number_lines_to_sample"])
        .agg(
            {
                "duration": "mean",
                "duration_symbol_table": "mean",
                "duration_compress_no_symbol": "mean",
                "compression_ratio": "mean",
            }
        )
        .reset_index()
    )

    # Sort the values
    fsst_with_fst_df = fsst_with_fst_df.sort_values(by="number_lines_to_sample")

    # For each number of lines to sample, add the values to the result_df as separate columns
    for number_lines_to_sample in fsst_with_fst_df["number_lines_to_sample"].unique():
        # First filter out the tuples of the number of lines to sample to create a temporary dataframe
        temp_df = fsst_with_fst_df[
            fsst_with_fst_df["number_lines_to_sample"] == number_lines_to_sample
        ]

        # Set the index to the input_file
        temp_df = temp_df.set_index("input_file")

        # Drop the column number_lines_to_sample
        temp_df = temp_df.drop(columns=["number_lines_to_sample"])

        # Calculate the compression factor
        temp_df = calculate_compression_factor(temp_df)

        # Calculate the compression speed in MB/s
        temp_df = calculate_compression_speed_in_mb_per_s(temp_df)

        # We want to add columns comparing the values of temp_df to fsst_without_fst
        temp_df = compare_values_to_fsst(temp_df, fsst_without_fst)

        # Rename the columns to add a suffix
        suffix = "fsst_with_fst"
        temp_df = temp_df.rename(
            columns={
                col: f"{col}_{suffix}" for col in temp_df.columns if col != "input_file"
            }
        )

        # Add suffix to the columns duration, duration_symbol_table, duration_compress_no_symbol and compression_ratio
        temp_df = temp_df.rename(
            columns={
                col: f"{col}_{str(int(number_lines_to_sample))}_lines_sampled"
                for col in temp_df.columns
                if col != "input_file"
            }
        )

        # Merge the temporary DataFrame to the result_df
        result_df = pd.merge(result_df, temp_df, on="input_file", how="outer")

    # Clean the underscores in input_file for easier use in LaTeX
    result_df["input_file"] = result_df["input_file"].str.replace("_", "")

    return result_df


def upper_quartile(x):
    return np.percentile(x, 75)


def lower_quartile(x):
    return np.percentile(x, 25)


def upper_whisker(x):
    q3 = np.percentile(x, 75)
    iqr = q3 - np.percentile(x, 25)
    return q3 + 1.5 * iqr


def lower_whisker(x):
    q1 = np.percentile(x, 25)
    iqr = np.percentile(x, 75) - q1
    return q1 - 1.5 * iqr


def preprocess_for_compression_ratio_per_input_file(input_file):
    # Filter out the tuples with the correct input_file
    filtered_df = df[(df["input_file"] == input_file) & (df["name"] == "fsst_with_fst")]

    # Calculate the compression factor
    filtered_df = calculate_compression_factor(filtered_df)

    # Calculate the average values for duration and compression_ratio
    filtered_df = (
        filtered_df.groupby(["name", "input_file", "number_lines_to_sample"])
        .agg(
            {
                "duration": "mean",
                "duration_symbol_table": "mean",
                "duration_compress_no_symbol": "mean",
                "compression_ratio": [
                    "mean",
                    "median",
                    "std",
                    lower_quartile,
                    upper_quartile,
                    lower_whisker,
                    upper_whisker,
                ],
                "compression_factor": [
                    "mean",
                    "median",
                    "std",
                    lower_quartile,
                    upper_quartile,
                    lower_whisker,
                    upper_whisker,
                ],
            }
        )
        .reset_index()
    )

    # Move the different aggregations of compression_ratio to the main level by renaming the columns
    filtered_df.columns = ["_".join(col).strip() for col in filtered_df.columns.values]

    # Rename some columns
    filtered_df = filtered_df.rename(
        columns={
            "name_": "name",
            "input_file_": "input_file",
            "number_lines_to_sample_": "number_lines_to_sample",
            "duration_mean": "duration",
            "duration_symbol_table_mean": "duration_symbol_table",
            "duration_compress_no_symbol_mean": "duration_compress_no_symbol",
            "compression_ratio_mean": "compression_ratio",
            "compression_ratio_median": "compression_ratio_median",
            "compression_ratio_std": "compression_ratio_std",
            "compression_ratio_lower_quartile": "compression_ratio_lower_quartile",
            "compression_ratio_upper_quartile": "compression_ratio_upper_quartile",
            "compression_ratio_lower_whisker": "compression_ratio_lower_whisker",
            "compression_ratio_upper_whisker": "compression_ratio_upper_whisker",
            "compression_factor_mean": "compression_factor",
            "compression_factor_median": "compression_factor_median",
            "compression_factor_std": "compression_factor_std",
            "compression_factor_lower_quartile": "compression_factor_lower_quartile",
            "compression_factor_upper_quartile": "compression_factor_upper_quartile",
            "compression_factor_lower_whisker": "compression_factor_lower_whisker",
            "compression_factor_upper_whisker": "compression_factor_upper_whisker",
        }
    )

    # Calculate the compression factor
    filtered_df = calculate_compression_factor(filtered_df)

    # Sort the values
    filtered_df = filtered_df.sort_values(by="number_lines_to_sample")

    # Save filtered_df to csv (for tikz in paper)
    clean_input_file = input_file.replace(
        "_", ""
    )  # Remove the underscores for easier use in LaTeX
    csv_filename = f"{clean_input_file}.csv"
    save_path = os.path.join(output_dir, "fsst_with_fst", csv_filename)
    os.makedirs(os.path.dirname(save_path), exist_ok=True)
    filtered_df.to_csv(save_path, index=False)


def analyze_compressed_files(input_file):
    # Get the file size of the input file in Bytes
    input_file_size = os.path.getsize(os.path.join(benchmark_dir, input_file))

    # For each run get the file size of the compressed files for fsst_without_fst
    # fsst_with_fst (100 lines sampled), and fsst_with_fst (200 lines sampled)
    fsst_without_fst_file_sizes = []
    fsst_with_fst_100_lines_sampled_file_sizes = []
    fsst_with_fst_200_lines_sampled_file_sizes = []

    for run in range(10):
        fsst_without_fst_file_size = os.path.getsize(
            os.path.join(
                compressed_files_dir,
                f"run_{run}",
                input_file,
                "fsst_without_fst.fsst",
            )
        )
        fsst_with_fst_100_lines_sampled_file_size = os.path.getsize(
            os.path.join(
                compressed_files_dir,
                f"run_{run}",
                input_file,
                "fsst_with_fst",
                "100_lines_sampled.fsst",
            )
        )
        fsst_with_fst_200_lines_sampled_file_size = os.path.getsize(
            os.path.join(
                compressed_files_dir,
                f"run_{run}",
                input_file,
                "fsst_with_fst",
                "200_lines_sampled.fsst",
            )
        )

        fsst_without_fst_file_sizes.append(fsst_without_fst_file_size)
        fsst_with_fst_100_lines_sampled_file_sizes.append(
            fsst_with_fst_100_lines_sampled_file_size
        )
        fsst_with_fst_200_lines_sampled_file_sizes.append(
            fsst_with_fst_200_lines_sampled_file_size
        )

    # Calculate the average file size of the compressed files
    fsst_without_fst_avg_file_size = statistics.mean(fsst_without_fst_file_sizes)
    fsst_with_fst_100_lines_sampled_avg_file_size = statistics.mean(
        fsst_with_fst_100_lines_sampled_file_sizes
    )
    fsst_with_fst_200_lines_sampled_avg_file_size = statistics.mean(
        fsst_with_fst_200_lines_sampled_file_sizes
    )

    # Calculate MiBs from the Bytes
    input_file_size_mib = input_file_size / 1024 / 1024
    fsst_without_fst_avg_file_size_mib = fsst_without_fst_avg_file_size / 1024 / 1024
    fsst_with_fst_100_lines_sampled_avg_file_size_mib = (
        fsst_with_fst_100_lines_sampled_avg_file_size / 1024 / 1024
    )
    fsst_with_fst_200_lines_sampled_avg_file_size_mib = (
        fsst_with_fst_200_lines_sampled_avg_file_size / 1024 / 1024
    )

    # Calculate the diff in compressed file size between fsst_without_fst and fsst_with_fst
    # (how much smaller is the compressed file size of fsst_with_fst compared to fsst_without_fst)
    diff_avg_file_size_100_lines_sampled = (
        -1
        * (
            1
            - (
                fsst_with_fst_100_lines_sampled_avg_file_size
                / fsst_without_fst_avg_file_size
            )
        )
        * 100
    )
    diff_avg_file_size_200_lines_sampled = (
        -1
        * (
            1
            - (
                fsst_with_fst_200_lines_sampled_avg_file_size
                / fsst_without_fst_avg_file_size
            )
        )
        * 100
    )

    return (
        input_file_size,
        input_file_size_mib,
        fsst_without_fst_avg_file_size,
        fsst_without_fst_avg_file_size_mib,
        fsst_with_fst_100_lines_sampled_avg_file_size,
        fsst_with_fst_100_lines_sampled_avg_file_size_mib,
        diff_avg_file_size_100_lines_sampled,
        fsst_with_fst_200_lines_sampled_avg_file_size,
        fsst_with_fst_200_lines_sampled_avg_file_size_mib,
        diff_avg_file_size_200_lines_sampled,
    )


############
# Main run #
############

if __name__ == "__main__":
    ###
    # Calculate the per input file .csv files for the standard deviation graphs
    ###

    # Read in the data
    df = pd.read_csv(input_csv)

    # For each input file, we want save the compression ratio
    unique_files = df["input_file"].unique()
    for file in unique_files:
        preprocess_for_compression_ratio_per_input_file(file)

    ###
    # Create the big preprocessed csv files (eav and normal) for the paper (used in the tables)
    ###

    # Get the unique benchmark files
    unique_files = df["input_file"].unique()

    # Create a result DataFrame
    result_df = pd.DataFrame()

    # For each file, get the preprocessed DataFrame
    for file in unique_files:
        # Append the preprocessed DataFrame to the result DataFrame
        result_df = result_df.append(get_preprocessed_df(df, file), ignore_index=True)

    # Remove the underscores in the column names for easier use in LaTeX
    result_df = result_df.rename(columns=lambda x: x.replace("_", ""))

    # Save the result_df to a csv file
    csv_filename = "preprocessed_benchmark_results.csv"
    save_path = os.path.join(output_dir, csv_filename)
    os.makedirs(os.path.dirname(save_path), exist_ok=True)
    result_df.to_csv(save_path, index=False)

    # Create an EAV-Version of the result_df
    eav_df = pd.melt(result_df, id_vars=["inputfile"])

    # Save the eav_df to a csv file
    csv_filename = "preprocessed_benchmark_results_eav.csv"
    save_path = os.path.join(output_dir, csv_filename)
    os.makedirs(os.path.dirname(save_path), exist_ok=True)
    eav_df.to_csv(save_path, index=False)

    # Prune the result_df to only contain the columns we want to use in the paper
    # The columns we want to use are: inputfile
    basic_columns = ["inputfile"]

    # Attributes we want
    attributes = [
        "compressionfactor",
        "compressionfactorcomparedtofsst",
        "durationsymboltable",
        "durationcompressnosymbol",
    ]

    # These attributes should be part of the pruned result_df for the following benchmarks:
    benchmarks = [
        "fsstwithoutfst",
        "fsstwithfstandfsstsampling",
        "fsstwithfst10linessampled",
        "fsstwithfst100linessampled",
        "fsstwithfst200linessampled",
    ]

    # Combine the attributes and benchmarks to get the columns we want to keep
    columns_to_keep = [
        f"{attribute}{benchmark}"
        for benchmark in benchmarks
        for attribute in attributes
    ]

    # Add the basic columns to the columns_to_keep
    columns_to_keep += basic_columns

    # Prune the result_df
    result_df_pruned = result_df[columns_to_keep]

    # Save the pruned result_df to a csv file
    csv_filename = "preprocessed_benchmark_results_pruned.csv"
    save_path = os.path.join(output_dir, csv_filename)
    os.makedirs(os.path.dirname(save_path), exist_ok=True)
    result_df_pruned.to_csv(save_path, index=False)

    # Create an EAV-Version of the pruned result_df
    eav_df_pruned = pd.melt(result_df_pruned, id_vars=["inputfile"])

    # Save the eav_df to a csv file
    csv_filename = "preprocessed_benchmark_results_pruned_eav.csv"
    save_path = os.path.join(output_dir, csv_filename)
    os.makedirs(os.path.dirname(save_path), exist_ok=True)
    eav_df_pruned.to_csv(save_path, index=False)

    ###
    # Analyse the compressed files to get some insights used in the paper's text
    ###

    result_df = pd.DataFrame(
        columns=[
            "Uncompressed File Size (Byte)",
            "Uncompressed File Size (MiB)",
            "Compressed File Size fsst_without_fst (Byte)",
            "Compressed File Size fsst_without_fst (MiB)",
            "Compressed File Size fsst_with_fst (100 lines sampled) (Byte)",
            "Compressed File Size fsst_with_fst (100 lines sampled) (MiB)",
            "Difference in Compressed File Size fsst_with_fst (100 lines sampled) compared to fsst_without_fst (%)",
            "Compressed File Size fsst_with_fst (200 lines sampled) (Byte)",
            "Compressed File Size fsst_with_fst (200 lines sampled) (MiB)",
            "Difference in Compressed File Size fsst_with_fst (200 lines sampled) compared to fsst_without_fst (%)",
        ]
    )

    for file in unique_files:
        result_df.loc[file] = analyze_compressed_files(file)

    # Save the result_df to a csv file
    csv_filename = "preprocessed_benchmark_compressed_files_analysis.csv"
    save_path = os.path.join(output_dir, csv_filename)
    os.makedirs(os.path.dirname(save_path), exist_ok=True)
    result_df.to_csv(save_path, index=True)
