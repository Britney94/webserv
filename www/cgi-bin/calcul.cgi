#!/usr/bin/env python3

import cgi

# Parse the form data
form = cgi.FieldStorage()

# Check if data was submitted
if "num1" not in form or "num2" not in form or "operation" not in form:
    print("<html>")
    print("<head>")
    print("<title>Error</title>")
    print("</head>")
    print("<body>")
    print("<h1>Error: Missing parameter</h1>")
    print("<p>Please provide values for 'num1', 'num2', and 'operation'.</p>")
    print("</body>")
    print("</html>")
    exit()

# Get the values of the form data
num1 = int(form.getvalue("num1"))
num2 = int(form.getvalue("num2"))
operation = form.getvalue("operation")

# Perform the requested operation
if operation == "+":
    result = num1 + num2
elif operation == "-":
    result = num1 - num2
elif operation == "*":
    result = num1 * num2
elif operation == "/":
    result = num1 / num2

# Print the result
print(f"""
<!DOCTYPE html>
<html>
<head>
    <title>CGI Calculator Result</title>
</head>
<body>
    <h1>Calculator</h1>
    <p>The result is: {result}</p>
</body>
</html>
""")