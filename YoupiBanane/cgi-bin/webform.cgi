#!/usr/bin/env python
import cgi

# Extract the data from the form
form = cgi.FieldStorage()

# Check if data was submitted
if "name" not in form or "email" not in form:
    print("<html>")
    print("<head>")
    print("<title>Error</title>")
    print("</head>")
    print("<body>")
    print("<h1>Error: Missing parameter</h1>")
    print("<p>Please provide values for 'name' and 'email'.</p>")
    print("</body>")
    print("</html>")
    exit()

# Extract the data from the form
name = form.getvalue("name")
email = form.getvalue("email")

# Print needed HTML
print("<html>")
print("<head>")
print("<title>CGI Form Response</title>")
print("</head>")
print("<body>")

# Process the data and return an HTML response
if name and email:
    print("<h2>Welcome, {}!</h2>".format(name))
    print("<p>Your email address, {}, has been successfully registered.</p>".format(email))
else:
    print("<h2>Error: Please fill in all fields on the form.</h2>")

# Print needed HTML
print("</body>")
print("</html>")
