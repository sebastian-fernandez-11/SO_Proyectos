import requests
from bs4 import BeautifulSoup
import os
import re

# URL de la página de libros populares del Proyecto Gutenberg
url = "https://www.gutenberg.org/browse/scores/top#books-last30"

# Directorio para guardar los libros
os.makedirs("libros_gutenberg", exist_ok=True)

# Hacer una solicitud a la página web
response = requests.get(url)
response.raise_for_status()  # Verifica que la solicitud sea exitosa

# Parsear el contenido de la página
soup = BeautifulSoup(response.text, "html.parser")

# Encontrar la sección "Top 100 EBooks last 30 days"
section = soup.find('h2', string="Top 100 EBooks last 30 days")
book_list = section.find_next('ol')

# Obtener los enlaces y títulos de los primeros 100 libros de la lista
book_info = []
for link in book_list.find_all('a', href=True):
    title = link.text
    href = "https://www.gutenberg.org" + link['href']
    book_info.append((title, href))

# Función para sanitizar nombres de archivo
def sanitize_filename(filename):
    # Remplazar espacios por guiones bajos
    filename = filename.replace(' ', '_')
    # Eliminar caracteres inválidos para nombres de archivo
    filename = re.sub(r'[\\/*?:"<>|]', "", filename)
    return filename

# Descargar cada libro en formato UTF-8
for i, (title, book_link) in enumerate(book_info, start=1):
    book_id = book_link.split("/")[-1]  # Extraer el ID del libro del enlace
    text_url = f"https://www.gutenberg.org/files/{book_id}/{book_id}-0.txt"
    
    try:
        book_response = requests.get(text_url)
        book_response.raise_for_status()

        # Sanitizar el título para usarlo como nombre de archivo
        sanitized_title = sanitize_filename(title)
        file_path = f"libros_gutenberg/{i}_{sanitized_title}.txt"

        # Guardar el libro en un archivo con el nombre del título
        with open(file_path, "wb") as file:
            file.write(book_response.content)

        print(f"Libro {i} descargado: {file_path}")

    except requests.HTTPError as e:
        print(f"No se pudo descargar el libro con ID {book_id}: {e}")

print("Descarga completada.")
