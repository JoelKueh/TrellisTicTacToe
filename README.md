Bluetooth Trellis
=================

Bluetooth trellis is a collection of libraries that make it easier to connect
external hardware with a computer over a bluetooth serial port via a PIC
microcontroller.

## PIC

On the PIC side, there are 5 relavent libraries: libi2c, libuart, libtrellis
liblcd, and libuarttrellis.

This code (as a whole) and more generally libuarttrellis is designed to
use the PIC as the controller for a rgb gamepad that you can connect to over
bluetooth. All of the other libraries are more general purpose.

The hardware used is as follows
* [Adafruit NeoTrellis RGB Driver](https://www.adafruit.com/product/3954?gad_source=1)
* [Adafruit Silicone Elastomer 4x4 Button Keypad](https://www.adafruit.com/product/3954?gad_source=1)
* [DSD Tech HC-06 Bluetooth Module](https://www.deshide.com/product-details.html?pid=344888&_t=1665215600)

The DSD Tech website for the HC-06 is incrediby finicky, so the link may be
broken by the time you read this. The amazon link is better (but the data on
it is innacurate) [Amazon DSD Tech HC-06](https://www.amazon.com/DSD-TECH-Wireless-Bluetooth-Transceiver/dp/B01FCQZ8VW/ref=sr_1_4?crid=208P4ZDIRYEOR&dib=eyJ2IjoiMSJ9.gciUocboeceE9ntTwzHYX7gzc7ixh5Z_nOpMri1-tgnW8DDNl__6LQRygtPZOXHjueVAFmNkqbBH68apFR5bVoy_tkBo1sK4WEV33K3jbAKIo_V9cZ7lbJpYsRbXcI9EgWoPs6RhQsQw5XTbaJcFVB4q_EFg5wjac7GmtylahSi06nuP4do8LwPVv3LVlToZrwl-2WcMSY3b1lVO68CEjQ.-Gvu3fvRYjdabhY1yzgEARRxwKuTnsx83GJZ15vCkHE&dib_tag=se&keywords=dsd+tech+hc-06&qid=1713815518&sprefix=dsd+tech+hc-0%2Caps%2C114&sr=8-4)

Libuarttrellis (the top level library) could be replaced with another library
that uses a different set of hardware (maybe it interfaces with an led matrix
or some other piece of hardware). This would be as simple as sending different
commands with different structures over libi2c and libuart.

In general, the code is designed to be extensible and reusable.

### libi2c

### libtrellis

### liblcd

### libuart

### libuarttrellis

## PC

On the PC side, there is are two libraries: libbluetoothserialport and
libbluetrellis.

Libbluetoothserialport is an extarnal library developed by
Agamnenztar that allows for multiplatform bluetooth serial port communication.
This allows us to write bluetooth code that works for any operating system.

Libbluetrellis (the PC equivalent of libuarttrellis on the PIC) serves as a
wrapper around libbluetoothserialport and provides a set of functions that
allow the user to interface with the PIC acting as an rgb gamepad.

### libbluetoothserialport

The documentation for libbluetoothserial port is contained on the original
github page by Agamnentzar. [bluetooth-serial-port](https://github.com/Agamnentzar/bluetooth-serial-port)

### libbluetrellis

Libbluetrellis, as was stated above is designed to be a wrapper around
libbluetoothserialport. The library exposes a series of functions that send
commands over the bluetooth bridge.

As was stated above, this library could be replaced or extended to allow for
interfacing with different hardware connected to the pic.

The library can be compiled with the option BLUE_STDIO=ON to rerout UART data
to STDIO. This forces the compiler to define BLUE_STDIO before compiling the
library hence the number of #ifndef's and #endif's in the source code.
To learn more about building look at README.md in the build directory.

#### Command Descriptions

The library defines the following commands (these correspond with the
libuarttrellis library on the PIC side)

```
enum {
    BUTTON_EVENT_HEADER = 'A',
    SHOW_HEADER = 'C',
    SET_LED_HEADER = 'D',
    SET_DISPLAY_HEADER = 'E',
    SET_LCD_HEADER = 'F',
};
```

```
/**
 * A struct that defines the layout of a button_event
 */
union button_event {
    struct {
        uint8_t button_num : 7;
        uint8_t is_rising : 1;
    };
    uint8_t raw;
};
```

```
/**
 * A struct that defines the layout of a set_led command
 */
union set_led {
    struct {
        uint8_t led_num;
        uint8_t color[3]; // RGB values for the led.
    };
    uint8_t raw;
};
```

```
/**
 * A struct that defines the layout of a set_display command
 */
union set_display {
    struct {
        uint8_t colors[16][3]; // Array of 16 GRB values.
    };
    uint8_t raw;
};
```

```
/**
 * A struct that defines the layout of a set_lcd comamnd
 */
union set_lcd {
    struct {
        uint8_t data[2][8]; // Array of strings for top and bottom row.
    };
    uint8_t raw;
};
```

#### Functions

The following functions are provided to simplify sending and recieving commands

```
/**
 * Constructs a blue_trellis object
 * @param addr a string containing the address of the bluetooth device
 */
blue_trellis(std::string addr);
```

```
/**
 * Sends a set_led command over bluetooth.
 * @param num The number of the led to set on the button pad
 * @param g The green value
 * @param r The red value
 * @param b The blue value
 */
void send_set_led(uint8_t num, uint8_t g, uint8_t r, uint8_t b);
```

```
/**
 * Sends a set_display command over bluetooth
 * @param colors An array of 16 GRB encoded colors to be displayed
 */
void send_set_display(const uint8_t colors[16][3]);
```

```
/**
 * Sends a set_lcd command over bluetooth
 * @param data The array of characters to be written to the lcd
 */
void send_set_lcd(const uint8_t data[2][8]);
```

```
/**
 * Non-blocking function that polls for a command header.
 * @return The header recieved (otherwise NULL)
 */
char poll_header();
```

```
/**
 * Performs a blocking wait for the body of a button_event
 * @return A struct containing the button event data.
 */
union button_event get_button_event();
```

```
/**
 * Destroys a blue_trellis object
 */
~blue_trellis();
```

#### Usage

Sending commands using this library is relatively self explanatory.
Calling different send functions queues up commands to be sent over the
bluetooth that will be sent and handled by the operating system.

The simplest example of this is in the reset_scene function from 
[trellis_main.cpp](https://github.com/JoelKueh/TrellisTicTacToe/blob/master/src/trellis_game/trellis_main.cpp)

```
void reset_scene(blue_trellis &bt)
{
	const uint8_t select_frame[16][3] = {
		{ 0x80, 0x00, 0x00 },
		{ 0x00, 0x80, 0x00 },
		{ 0x00, 0x00, 0x80 },
		{ 0x80, 0x80, 0x80 },
		{ 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00 }
	};

	const uint8_t blank_lcd[2][8] = {
		{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
		{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' }
	};

	bt.send_set_display(select_frame);
	bt.send_set_lcd(blank_lcd);
}
```

This function simply resets the trellis display and lcd to the values
defined in the constants above.

A more complicated set example can be found in the draw_frame() funciton from
[dummy_animation.cpp](https://github.com/JoelKueh/TrellisTicTacToe/blob/master/src/trellis_game/dummy_animation.cpp)

```
void dummy_animation::draw_frame(int number)
{
	const uint8_t color[3] = { 0xFF, 0xFF, 0xFF };
	const uint8_t numbers[12] = { 0, 1, 2, 3, 7, 11, 15, 14, 13, 12, 8, 4};
	uint8_t frame[16][3] = { 0 };
	int i, j, end;

	i = number % 12;
	j = 11;
	end = (number + 11) % 12;
	while (i != end) {
		frame[numbers[i]][0] = color[0] / j;
		frame[numbers[i]][1] = color[1] / j;
		frame[numbers[i]][2] = color[2] / j;
		i = i + 1 == 12 ? 0 : i + 1;
		j--;
	}
	bt->send_set_display(frame);
}
```

This function mathematically defines an animation as a series of frames that
can be sent to the display. It is called every 50 milliseconds from the update
method in [dummy_animation.cpp](https://github.com/JoelKueh/TrellisTicTacToe/blob/master/src/trellis_game/dummy_animation.cpp)

It is important to note the delay that is put in between frames. If a device
attempts to send too many commands over bluetooth, the operating systems
internal transmission buffer will become full and will throw an exception.
In general, frames should be sent with a period no less than 20ms.

```
void dummy_animation::update()
{
	// Get the time between this frame and the last
	auto now = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(now - *last_time);

	// If 500 milliseconds have passed, update the time.
	if (duration.count() > 50) {
		draw_frame(frame_count++);

		// Update our last frame time
		delete last_time;
		last_time = new high_resolution_clock::time_point(now);
	}

	// Delete button presses in the buffer so we don't have a bunch
	// queued when this animation ends.
	consume_button_presses();
}
```

Handling button events is not much more difficult than writing. One simply uses
the function poll_header() to check for data in the buffer. Then they call
the corresponding get command to get the relevant data. From that point, you
can do whatever you want with the data.

This is even more simple because there is only one type of command that is
recieved on the PC side: a button event.

For more details see the example from select_scene() in [trellis_main.cpp](https://github.com/JoelKueh/TrellisTicTacToe/blob/master/src/trellis_game/trellis_main.cpp). The function gets the header and uses it
to select the next scene that will be displayed on the trellis.

```
scene *select_scene(blue_trellis &bt)
{
	char header = bt.poll_header();
	union blue_trellis::button_event event;

	if (header == blue_trellis::BUTTON_EVENT_HEADER) {
		event = bt.get_button_event();
		return get_scene(&bt, event);
	}

	return nullptr;
}
```
