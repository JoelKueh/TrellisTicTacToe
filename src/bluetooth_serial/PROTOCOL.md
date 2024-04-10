Bluetooth UART Protocol
=======================

Sending
-------

A command character is sent first to notify the reciever of the command type.
The command for the data is then sent in 4-byte packets preceeded by a pipe.
This sequence will then be terminated by a backslash.

The command sent is always followed by a 2-byte checksum hash.
Format: <COM_ID>|xxxx|xxxx|xxxx|xxxx\<CHECKSUM>

These commands are pushed to a transmit buffer before they are sent over
the UART interface. The command is removed only when it has been successfully
verified.

A command send may be interrupted by an acknowledge sequence. This acknowledge
sequence will consist of either five plusses or five minuses (+++++/-----).
Five plusses signifies that the last command sent was successful.

If a "-----" sequence is ever recieved, the current command should be resent
in its entirity after a delay of 25ms since the last character sent.

If this acknowledge is not recieved within 25ms of the trasmission of the last
character, the command should also be resent in its entirity.

Recieving
---------

Commands are described by a struct and consist of an identifying character,
a data length, and a pointer to a callback function.

Incoming data is moved to a 512-Byte circular buffer. A push to this buffer
will be completed only when the checksum has been verified.

If at any point 10ms pass between the reception of two characters, the command
should be considered void and the 

Error Handling
--------------

Due to the fact that we are transmitting over bluetooth, the bit error rate
is relatively high. To guarantee robust communication, we must verify that
the command recieved is the same as the command that was sent.

Bit errors are handled by the checksum at the end of the command. Any other
errors 
