(TeX-add-style-hook
 "portofoliu"
 (lambda ()
   (TeX-add-to-alist 'LaTeX-provided-class-options
                     '(("report" "11pt" "a4paper")))
   (TeX-add-to-alist 'LaTeX-provided-package-options
                     '(("inputenc" "utf8") ("fontenc" "T1") ("babel" "romanian")))
   (add-to-list 'LaTeX-verbatim-environments-local "cpp*")
   (add-to-list 'LaTeX-verbatim-environments-local "cpp")
   (add-to-list 'LaTeX-verbatim-environments-local "minted")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "hyperref")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "hyperimage")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "hyperbaseurl")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "nolinkurl")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "url")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "path")
   (add-to-list 'LaTeX-verbatim-macros-with-delims-local "path")
   (TeX-run-style-hooks
    "latex2e"
    "report"
    "rep11"
    "minted"
    "mathtools"
    "amssymb"
    "inputenc"
    "fontenc"
    "enumitem"
    "geometry"
    "babel"
    "hyperref"
    "sectsty"
    "xparse"
    "tocloft")
   (TeX-add-symbols
    '("commonFile" 2)
    '("labtitle" 2)
    '("lab" 1)
    '("mat" 1)
    '("cppcode" 1)
    "matmn"
    "matn")
   (LaTeX-add-labels
    "#2")
   (LaTeX-add-environments
    "problema"))
 :latex)

