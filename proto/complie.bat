mkdir cpp
del .\cpp\* /s /q /f
protoc --cpp_out=.\cpp class.proto
protoc --cpp_out=.\cpp student.proto
protoc --cpp_out=.\cpp user.proto
protoc --cpp_out=.\cpp task.proto
protoc --cpp_out=.\cpp net.proto