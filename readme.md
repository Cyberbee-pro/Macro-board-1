# 🛠️ Macro Board V1

![Full Banner](Images/Frame%201.png)

A highly programmable, multi-layered custom hardware macro pad and controller built around the **ESP32** architecture. This device features a full 3x3 key matrix, a mouse-emulating analog joystick, an absolute analog potentiometer dial, and dedicated mode-switching indicators.

---

## 📸 Visual References

### 3D Enclosure Render
![En enclosure 3D Model Render](Images/render%202.jpg)

### Wokwi Circuit Layout
<p align="center">
   <img src="Images/wofi%20render.png" alt="Wokwi Circuit Render Layout" width="450" />
</p>

---

## 🚀 Features

* **🎛️ Absolute Knob Control:** Integrated $10\text{k}\Omega$ potentiometer for fluid scrolling, volume adjustment, or continuous parameter trimming.
* **🕹️ Joystick Mouse Emulation:** Dual-axis analog stick for smooth cursor tracking with an integrated **Select (SEL)** click action.
* **🧱 3x3 Tactile Key Matrix:** 9 fully programmable mechanical macro keys mapped for high-speed shortcuts or custom key injection profiles.
* **🔴 Dedicated Left & Right Click Action:** Specialized physical buttons separated for quick peripheral interaction.
* **🔄 Advanced Mode Switching:** On-the-fly profiling via active hardware toggles to switch macro layers instantly.
* **💡 Mode Bulbs:** 3 hardware status LEDs to provide immediate visual feedback on the currently active macro profile layer.

---

## 📦 Bill of Materials (BOM)

| Qty | Component Description | Purpose |
| :---: | :--- | :--- |
| 1 | **ESP32 Development Board** (30-Pin / 38-Pin Module footprint) | Core Processing Engine |
| 11 | **6mm Tactile Pushbuttons** | Key Matrix & Macro Switches |
| 1 | **$10\text{k}\Omega$ Rotary Potentiometer** | Continuous Analog Value Adjustment |
| 1 | **Dual-Axis Analog Joystick Module** | Mouse/Cursor Control Emulation |
| 3 | **Standard 5mm LEDs (Red)** | Profile Layer Indicators |
| 1 | **Half-Size Breadboard** (or Custom Prototyping PCB) | Circuit Infrastructure Base |

---

## ⚡ Hardware Pin Allocation Mapping Matrix

This hardware definition layout maps your physical workspace directly to the core silicon registers, keeping all assignments clear of the internal flash memory and strapping zones.

### 1. Analog Input Controls (ADC1 registers)
* **Potentiometer Signal (`pot1:SIG`):** `GPIO 35`
* **Joystick Vertical Axis (`VERT`):** `GPIO 36` (VP)
* **Joystick Horizontal Axis (`HORZ`):** `GPIO 39` (VN)

### 2. Status Output LEDs
* **LED 4 Anode:** `GPIO 25`
* **LED 5 Anode:** `GPIO 33`
* **LED 6 Anode:** `GPIO 32`

### 3. Functional Toggles
* **Yellow Mode Selector (`btn11`):** `GPIO 26`
* **Red Function Switch (`btn12`):** `GPIO 14`
* **Joystick Center Click (`joystick1:SEL`):** `GPIO 27`

### 4. Core 3x3 Key Matrix Layout
// --- Row 1 (TOP Layer) ---
* **KEY_TOP_LEFT**      `GPIO 21`
* **KEY_TOP_MID**       `GPIO 22`  
* **KEY_TOP_RIGHT**     `GPIO 23`

// --- Row 2 (Middle Layer) ---
* **KEY_MID_LEFT**       `GPIO 5` 
* **KEY_MID_MID**        `GPIO 18`
* **KEY_MID_RIGHT**      `GPIO 19`  

// --- Row 3 (Bottom Layer) ---
* **KEY_BOT_LEFT**       `GPIO 4`  
* **KEY_BOT_MID**        `GPIO 16`
* **KEY_BOT_RIGHT**      `GPIO 17`