#!/bin/bash


mkdir -p lib
mkdir -p target

# Download JUnit if not present
if [ ! -f "lib/junit-4.13.2.jar" ]; then
    echo "Downloading JUnit..."
    curl -L -o lib/junit-4.13.2.jar https://repo1.maven.org/maven2/junit/junit/4.13.2/junit-4.13.2.jar
fi

if [ ! -f "lib/hamcrest-core-1.3.jar" ]; then
    echo "Downloading Hamcrest..."
    curl -L -o lib/hamcrest-core-1.3.jar https://repo1.maven.org/maven2/org/hamcrest/hamcrest-core/1.3/hamcrest-core-1.3.jar
fi

# Clean
if [ -z "$1" ]; then
    rm -rf target/*
fi

# Check jq
if ! command -v jq &> /dev/null
then
    echo "jq could not be found. Please install it to run this script."
    echo "On macOS, you can use Homebrew: brew install jq"
    echo "On Debian/Ubuntu, you can use: sudo apt-get install jq"
    exit 1
fi



CLASSPATH="lib/junit-4.13.2.jar:lib/hamcrest-core-1.3.jar"


process_subproject() {
    local subproject=$1
    echo "=================================================="
    echo "Processing subproject: $subproject"
    
    SUBFOLDER=$(jq -r ".exercices[\"$subproject\"].required[0] | split(\"/\") | .[:-1] | join(\"/\")" desc.json)
    echo "Subfolder: $SUBFOLDER"

    echo -e "==================================================\n"

    echo "Compiling..."

    # Compile all Scala files in the subfolder
    find "$SUBFOLDER" -name "*.scala" | xargs scalac -d target -cp "$CLASSPATH:target"

    if [ $? -ne 0 ]; then
        echo -e "Compilation failed. Skipping tests for this subproject.\n"
        return 1
    else
        echo -e "Compilation successful.\n"

        # Run tests
        echo "Running tests..."
        TEST_CLASSES=$(jq -r ".exercices[\"$subproject\"].tests[]" desc.json | tr '\n' ' ')
        scala -cp "target:$CLASSPATH" org.junit.runner.JUnitCore $TEST_CLASSES

        echo "All tests finished."
    fi
}

# Check if a specific subproject is provided
if [ -n "$1" ]; then
    ARG="$1"
    PROJECT_NAME=""

    # Try to find a match for the argument
    # Direct match
    if jq -e ".exercices | has(\"$ARG\")" desc.json > /dev/null; then
        PROJECT_NAME="$ARG"
    fi

    # Match by number
    if [ -z "$PROJECT_NAME" ] && [[ "$ARG" =~ ^[0-9]+$ ]]; then
        PROJECT_NAME=$(jq -r ".exercices | keys[] | select(startswith(\"$ARG-\"))" desc.json)
    fi

    # Match by subfolder
    if [ -z "$PROJECT_NAME" ]; then
        PROJECT_NAME=$(jq -r --arg arg "$ARG" '.exercices | to_entries[] | select(.value.required[0] | contains($arg)) | .key' desc.json)
    fi

    if [ -n "$PROJECT_NAME" ]; then
        # Check for ambiguity
        if [ "$(echo "$PROJECT_NAME" | wc -l)" -gt 1 ]; then
             echo "Error: Ambiguous argument '$ARG'. Multiple projects found:"
             echo "$PROJECT_NAME"
             exit 1
        fi
        process_subproject "$PROJECT_NAME"
    else
        echo "Error: Subproject '$ARG' not found in desc.json."
        echo "You can use the project number, subfolder name, or full name."
        echo "Available subprojects are:"
        jq -r '.exercices | keys[]' desc.json
        exit 1
    fi
else
    # Get all subprojects from desc.json
    jq -r '.exercices | keys[]' desc.json | while IFS= read -r subproject; do
        process_subproject "$subproject"
    done
fi