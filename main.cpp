#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <opencv2/opencv.hpp>

int main()
{
    // --- Step 1: Open the input file ---
    std::ifstream file("result.txt");
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open results.txt" << std::endl;
        return -1;
    }

    std::string line;
    std::string current_filename = "";
    cv::Mat current_image;

    std::cout << "Starting batch processing..." << std::endl;

    // --- Step 2: Read file line by line ---
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string filename;
        int x, y, w, h;
        iss >> filename >> x >> y >> w >> h;

        // --- Step 3: Check if we need to switch images ---
        if (filename != current_filename)
        {
            // If an image was already being worked on, save it first
            if (!current_image.empty())
            {
                std::string prev_output_path = "blurred_images/blurred_" + current_filename;
                cv::imwrite(prev_output_path, current_image);
                std::cout << "  -> Saved " << prev_output_path << std::endl;
            }

            // Load the new image
            current_filename = filename;
            std::string new_image_path = "data/" + current_filename;
            current_image = cv::imread(new_image_path);

            if (current_image.empty())
            {
                std::cerr << "Warning: Could not load " << new_image_path << ". Skipping." << std::endl;
            }
            else
            {
                std::cout << "Processing " << current_filename << "..." << std::endl;
            }
        }

        // --- Step 4: Apply blur to the current line's ROI ---
        if (!current_image.empty())
        {
            // Define the rectangle for the blur
            cv::Rect roi_to_blur(x, y, w, h);

            // Apply the blur directly to that part of the image
            cv::GaussianBlur(current_image(roi_to_blur), current_image(roi_to_blur), cv::Size(31, 31), 0);
        }
    }

    // --- Step 5: Save the very last image after the loop is done ---
    if (!current_image.empty())
    {
        std::string last_output_path = "blurred_images/blurred_" + current_filename;
        cv::imwrite(last_output_path, current_image);
        std::cout << "  -> Saved " << last_output_path << std::endl;
    }

    file.close();
    std::cout << "\nBatch processing complete." << std::endl;
    return 0;
}