#!/usr/bin/env python3
import os
import io
import base64
from PIL import Image

# Get the content type of the request
content_type = os.environ.get('CONTENT_TYPE', '')

if content_type == 'multipart/form-data':
    # Get the content length of the request
    content_length = os.environ['CONTENT_LENGTH']
    content_length = int(content_length) if content_length else 0

    # Get the path to the uploaded file
    path_translated = os.environ['PATH_TRANSLATED']

    # Read the raw data from the request
    query_string = os.environ['QUERY_STRING']

    # Go to the good directory
    os.chdir("./")

    # Extract the text and image data from the raw data
    text_data = query_string.split('text=')[1].split('&')[0]
    image_data = query_string.split('filename=')[1]

    # Write the text data to a file
    file_list = path_translated.strip().split('\n')

    # Check if the path translated is empty
    if path_translated == '':
        print('<html><head><title>CGI Multipart</title></head><body>')
        print('<h1>CGI Multipart</h1>')
        print('<p>Error: No file uploaded</p>')
        print('</body></html>')
        exit()

    # Print a response to the client
    print('<html><head><title>CGI Multipart</title></head><body>')
    print('<h1>CGI Multipart</h1>')
    print('<p>Path translated: {}</p>'.format(path_translated))
    print('<table>')
    for file_path in file_list:
        with open(file_path, 'rb') as f:
            file_content = f.read()
        print('<p>File path: {}</p>'.format(file_path))
        if '.svg' in image_data:
            print(file_content.decode('utf-8'))
        elif '.txt' in image_data:
            print('<tr><td><p>{}</p></td></tr>'.format(file_content.decode('utf-8')))
        elif '.jpeg' in image_data or '.jpg' in image_data:
            # Load the image using PIL
            image = Image.open(io.BytesIO(file_content))
            # Convert the image to JPEG and encode it as base64
            buffered = io.BytesIO()
            image.convert('RGB').save(buffered, format="JPEG")
            encoded_image = base64.b64encode(buffered.getvalue()).decode()
            # Generate the HTML code to display the image
            html = '<img src="data:image/jpeg;base64,{}">'.format(encoded_image)
            print('<tr><td>{}</td></tr>'.format(html))
        elif '.base64' in image_data:
            source = "data:image/png;base64," + file_content.decode('utf-8')
            print('<tr><td><img src="{}" alt="Image"></td></tr>'.format(source))
        else:
            print('<tr><td><p>Unsupported file type</p></td></tr>')
    print('</table>')
    print('</body></html>')

else:
    print('<html><head><title>CGI Multipart</title></head><body>')
    print('<h1>CGI Multipart</h1>')
    print('<p>Error: Unsupported content type</p>')
    print('</body></html>')