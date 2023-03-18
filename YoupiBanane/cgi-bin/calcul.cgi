#!/usr/bin/env python3

import cgi

# Parse the form data
form = cgi.FieldStorage()

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
    <title>Calculator Result</title>
</head>
<body>
    <h1>Calculator</h1>
    <p>The result is: {result}</p>
</body>
</html>
""")