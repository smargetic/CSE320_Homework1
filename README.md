# CSE320_Homework_1

#For this assignment, we serialize and deserialize the contentents of a directory, including subdirectories and so forth.

#The user provides instuctions in the form of "-s, -d, -h", which represent "serialize," "deserialize," and "help" respectively.
#Additional instructions include, "-p, -c" which represents "path," providing an alternative path, and "clobber."
#The validargs function identifies which inputs are acceptable.
#The path is changed with the argument following "-p," using stack methodology.

#There are six different record types: Start of Transmission, End of Transmission, Start of Directory, End of Directory, Directory Entry, and File Data.
#Each record type consists of a header, which consists of 3 magic bytes, "0x0C 0x0D 0xED," the type (enum value of record types listed above), the depth, and the size (typically 16, if no additional information is attached).
#Start of Transmission marks the beginning of the serialization.
#End of Transmission marks the end of serialization.
#Start of Directory marks the beginning of a directory entry.
#End of Directory marks the end of a directory.
#Directory Entry marks any entry within a directory (can be file or another directory). The size of the directory entry consists of an addition 12, and sizeof(nameOfEntry).
#File Data marks a file to be read. The header size will consist of both the typical size of the header, and the size of the data of the file.
#Following this header, will be the serialized data of the file.

#The function filesRecursively serializes the contents of a file recursively, as described above.

#The deserialize portion of the assignment is the reverse of the serialization portion.
#We are given serialized data, and have to create the files and directories, with their contents accordingly.
