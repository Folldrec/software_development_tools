set -e  

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' 

print_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

check_doxygen() {
    print_info "Checking for Doxygen..."
    
    if ! command -v doxygen &> /dev/null; then
        print_error "Doxygen is not installed!"
        echo ""
        echo "Please install Doxygen:"
        echo "  Ubuntu/Debian: sudo apt-get install doxygen graphviz"
        echo "  macOS: brew install doxygen graphviz"
        echo "  Windows: choco install doxygen.install graphviz"
        exit 1
    fi
    
    print_success "Doxygen found: $(doxygen --version)"
}

check_doxyfile() {
    print_info "Checking for Doxyfile..."
    
    if [ ! -f "Doxyfile" ]; then
        print_error "Doxyfile not found in current directory!"
        echo ""
        echo "Please run this script from the project root directory."
        exit 1
    fi
    
    print_success "Doxyfile found"
}

clean_docs() {
    print_info "Cleaning previous documentation..."
    
    if [ -d "docs" ]; then
        rm -rf docs/
        print_success "Removed old documentation"
    else
        print_info "No previous documentation to clean"
    fi
}

generate_docs() {
    print_info "Generating documentation..."
    echo ""
    
    if doxygen Doxyfile 2>&1 | tee doxygen.log; then
        print_success "Documentation generated successfully!"
    else
        print_error "Documentation generation failed!"
        print_info "Check doxygen.log for details"
        exit 1
    fi
    
    echo ""
    
    warning_count=$(grep -c "warning:" doxygen.log || true)
    
    if [ "$warning_count" -gt 0 ]; then
        print_warning "Found $warning_count warning(s) during generation"
        echo ""
        echo "Warnings:"
        grep "warning:" doxygen.log
        echo ""
    else
        print_success "No warnings found!"
    fi
}

show_stats() {
    print_info "Documentation statistics:"
    echo ""
    
    if [ -d "docs/html" ]; then
        html_files=$(find docs/html -name "*.html" | wc -l)
        echo "  📄 HTML files: $html_files"
        
        if [ -f "docs/html/index.html" ]; then
            file_size=$(du -h docs/html | tail -1 | cut -f1)
            echo "  💾 Total size: $file_size"
        fi
        
        echo ""
        print_success "Documentation is ready!"
        echo "  📂 Location: $(pwd)/docs/html/"
        echo "  🌐 Main page: $(pwd)/docs/html/index.html"
    else
        print_error "Documentation directory not found!"
        exit 1
    fi
}

open_docs() {
    print_info "Opening documentation in browser..."
    
    index_file="docs/html/index.html"
    
    if [ ! -f "$index_file" ]; then
        print_error "index.html not found!"
        exit 1
    fi
    
    case "$(uname -s)" in
        Linux*)
            if command -v xdg-open &> /dev/null; then
                xdg-open "$index_file"
            else
                print_warning "Could not open browser automatically"
                echo "Please open: file://$(pwd)/$index_file"
            fi
            ;;
        Darwin*)
            open "$index_file"
            ;;
        CYGWIN*|MINGW*|MSYS*)
            start "$index_file"
            ;;
        *)
            print_warning "Unknown OS, could not open browser"
            echo "Please open: file://$(pwd)/$index_file"
            ;;
    esac
}

main() {
    echo ""
    echo "Doxygen Documentation Generator:"
    echo ""
    
    check_doxygen
    check_doxyfile
    echo ""
    
    clean_docs
    generate_docs
    show_stats
    echo ""
    
    if [ "$1" == "--open" ] || [ "$1" == "-o" ]; then
        open_docs
    else
        echo ""
        print_info "To open documentation in browser, run:"
        echo "  ./generate_docs.sh --open"
    fi
    
    echo ""
    print_success "Done!"
    echo ""
}

main "$@"