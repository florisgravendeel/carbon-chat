import subprocess
import os
carbon_folder = os.path.dirname(os.path.realpath(__file__)) + "/carbon-lang/" 
process = subprocess.Popen("bazel run //explorer -- ../main.carbon", stdout=subprocess.PIPE, shell=True, cwd=carbon_folder)
print("Starting server")
print(process.communicate())