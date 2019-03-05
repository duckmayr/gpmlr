library(testthat)

# To test whether attaching works:
attach_message <- capture.output(library(gpmlr), type = "message")
test_check("gpmlr")

