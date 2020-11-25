; Historisch bedingt darf man bei einem normalen BIOS einen ordentlichen
; Eiertanz aufführen, bis man endlich den eigentlichen Kernel erreicht
; (angefangen bei nur 512 Byte im Master Boot Record...)
; Zum Glück gibt es Bootloader (vgl https://wiki.osdev.org/Bootloader), die uns
; diese Arbeit (zum Teil) abnehmen:
; Sie laden unseren Kernel in den Speicher, wechseln (mit einem Prozessor, dem
; Betriebssystemprozessor) in den Protected Mode (32 Bit) und springen an den
; Anfang unseres Kernels -- viel langweilige Arbeit (Doku lesen) gespart.
; Einer der bekanntesten Vertreter ist GRUB, welcher auch den Multiboot-Standard
; definiert hat (https://www.gnu.org/software/grub/manual/multiboot/multiboot.html)
; Hierbei wird uns auch noch eine Datenstruktur übergeben, welche z.B.
; Verweise auf weitere Module (wie der Initialen Ramdisk) beinhaltet.
; Wir müssen dafür nur am Anfang (in den ersten 8192 Bytes) einen Header mit
; "magischen" Werten definieren.

; vordefinierte Multiboot-Konstanten (vgl Multiboot-Standard)

; Magischer Header, welcher im Kernel stehen muss
MULTIBOOT_HEADER_MAGIC_OS equ 0x1badb002

; Antowrt auf magischen Header, welcher vom Bootloader in eax geschrieben wird
MULTIBOOT_HEADER_MAGIC_LOADER equ 0x2badb002

; Flag, damit zusätzliche, in der Bootkonfiguration angegebenene Module (wie
; die initiale Ramdisk) an den Seitenadressen (4 KB) ausgerichtet werden
MULTIBOOT_PAGE_ALIGN equ 1<<0

; Flag, damit Speicher Informationen über die Multiboot-Datenstruktur an das
; Betriebssystem übergeben werden
MULTIBOOT_MEMORY_INFO equ 1<<1

; Flag, damit der Videomodus gesetzt werden kann
MULTIBOOT_VIDEO_MODE equ 1<<2

; Konfiguration der gewünschten Grafikeinstellungen (bei gesetztem Videomodus)
MULTIBOOT_VIDEO_WIDTH equ 1280
MULTIBOOT_VIDEO_HEIGHT equ 1024
MULTIBOOT_VIDEO_BITDEPTH equ 32

; Die von uns gewünschten Flags
MULTIBOOT_HEADER_FLAGS equ MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO

; Prüfsumme
MULTIBOOT_HEADER_CHKSUM equ -(MULTIBOOT_HEADER_MAGIC_OS + MULTIBOOT_HEADER_FLAGS)
