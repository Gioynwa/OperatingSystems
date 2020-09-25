Paraskevi Bakami 2117
Dimitrios Kladouchos 2189
Georgios Simos 2180

Με την ένταξη του Best Fit στον κωδικα του slob.c 
αναμένουμε να δούμε πολύ αργή απόκριση κατά τη διάρκεια 
του boot process αλλά και μετά απο αυτό. 
Αυτό συμβαίνει επειδή o Best Fit διατρέχει όλη τη λίστα 
των σελίδων αλλά και των blocks αναζητώντας το καλύτερο 
ταίριασμα ενώ οι Next Fit-First Fit επιλέγουν άμεσα την 
σελιδα για allocation. Παρόλα αυτά ο Best Fit αξιοποιεί 
πιο αποδοτικά τον διαθέσιμο χώρο στη μνήμη.

compilation for application slob_get_total_mem_lib:
->make
->./slob_get_total_mem_lib

