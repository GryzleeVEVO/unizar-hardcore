package check

import (
	"fmt"
	"os"
)

func CheckError(err error, comment string) {
	if err != nil {
		fmt.Fprintf(os.Stderr, "In: %s, Fatal error: %s", comment, err.Error())
		os.Exit(1)
	}
}

func CheckError2(err error, where string, what string) {
	if err != nil {
		fmt.Fprintf(os.Stderr, "[ERROR] [%s] %s: %s",
			where, what, err.Error())
		os.Exit(1)
	}
}
