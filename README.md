📸 ESP32-CAM Viewer (Cheap Yellow Display - CYD)
This project allows you to view a snapshot from a motionEye camera (Raspberry Pi) on a 2.4-inch ESP32-2432S024 display.

⚠️ Challenges and Technical Solutions
During development, we identified that the standard ESP32 (without PSRAM) has RAM limitations for processing high-resolution JPEG images (640x480). To solve this, we implemented:

Optimized Memory Buffer: We limited the receive buffer to 60KB to avoid the malloc failed error.

Hardware Scaling (Scale 2): We used the TJpg_Decoder library to reduce the image from 640x480 to 320x240 (the display's native resolution) during decoding, saving processing power.

Quality Adjustment (The Critical Point): The JPEG file from the server must have a maximum quality of 75%. If the quality is higher (100%), the file size will exceed the ESP32's RAM, and the image will appear cropped or with artifacts.

🛠️ motionEye Configuration
To allow the ESP32 to display the entire image, configure your camera in motionEye as follows:

Video Resolution: You can keep it at 640x480 (for high-quality recordings).

Snapshot URL: Use the endpoint /picture/1/current/.

JPEG Quality: Adjust to 75% or less. This reduces the file size in bytes without affecting the pixel resolution, allowing the ESP32 to process the complete frame.

💻 Dependencies
- TFT_eSPI (Configured for the CYD ILI9341 driver).
- TJpg_Decoder (For fast JPEG decoding).

Developed with the help of Google AI Gemini.
