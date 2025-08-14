import cv2
import os
def detect_faces(image_path, model):
    """
    Finds all faces in an image and returns their bounding box coordinates.

    Args:
        image_path (str or Path): The path to the input image.
        model (face detection model).

    Returns:
        A list of bounding boxes [x, y, w, h] for any detected faces.
        Returns an empty list if no faces are found or the image fails to load.
    """
    # Read the image
    image = cv2.imread(str(image_path))
    if image is None:
        print(f"Warning: Could not read image at {image_path}")
        return []
    # Convert to grayscale for the detector
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    # Detect faces and return the coordinates
    faces = model.detectMultiScale(
        gray,
        scaleFactor=1.1,
        minNeighbors=5,
        minSize=(30, 30)
    )
    return faces


def main(path):
    # Initialize the model
    model = cv2.CascadeClassifier(
        cv2.data.haarcascades + 'haarcascade_frontalface_default.xml'
    )
    # Write to output file
    output_file="result.txt"
    with open(output_file, "w") as file:
        for im in os.listdir(path):
            if im.lower().endswith(".jpg"):
                im_path = os.path.join(path,im)
                faces = detect_faces(image_path=im_path, model=model) # Detection
                for (x, y, w, h) in faces:
                    file.write(f"{im} {x} {y} {w} {h}\n")

if __name__ == "__main__":
    # Change to your current path with data
    main(r"...\blur\data")
