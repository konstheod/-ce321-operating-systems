Καράκουλα Κατερίνα		1604	karakoul@uth.gr
Θεοδοσίου Κωνσταντίνος   	1619	konstheo@uth.gr
Πέγιος Κωνσταντίνος		1664	kopegios@uth.gr

Για το syscall:
Αρχικα δημιουργησαμε την στατικη βιβλιοθηκη με την συναρτηση find_roots_wrapper().
Δημιουργησαμε το roots.c οπου υλοποιει την συναρτηση find_roots_wrapper().

Εκτελεσαμε τις παρακατω εντολες με τη σειρα:
gcc -c roots.c -o roots.o
ar rcs libroots.a roots.o
gcc -g -Wall find_roots_lib.c -o find_roots_lib -L. -lroots

Για το module:
Εφοσον ειμαστε στον φακελο οπου βρισκεται το module "project1-iosched.c" εκτελουμε την εντολη sudo make 
σε ενα terminal.
Για να εισαγουμε το module εκτελουμε sudo insmod project1-iosched.ko
Επισης το ονομα του I/O scheduling module είναι "team_1604_1619" (δεν χωρουσε το τριτο αεμ της ομαδας μας και μας εβγαζε σκουπιδια,
λογω του περιορισμου του Ν=16 στην βιβλιοθηκη elevator.h ).