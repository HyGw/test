<!DOCTYPE style-sheet PUBLIC "-//James Clark//DTD DSSSL Style Sheet//EN" [
<!ENTITY % html "IGNORE">
<![%html;[
<!ENTITY % print "IGNORE">
<!ENTITY docbook.dsl PUBLIC "-//Norman Walsh//DOCUMENT DocBook HTML Stylesheet//EN" CDATA dsssl>
]]>
<!ENTITY % print "INCLUDE">
<![%print;[
<!ENTITY docbook.dsl PUBLIC "-//Norman Walsh//DOCUMENT DocBook Print Stylesheet//EN" CDATA dsssl>
]]>
]>

<style-sheet>

<style-specification id="print" use="docbook">
<style-specification-body>

<!-- See docbook-dsssl-1.x/doc -->
(define %generate-reference-toc% #t)
(define %refentry-new-page% #t)
(define %number-programlisting-lines% #f)
(define %section-autolabel% #t)
(define bop-footnotes #t)
<!-- (define %show-comments% #f) -->
(define ($generate-book-lot-list$)
  (list (normalize "figure")
    (normalize "example")))
<!-- A4 | USletter -->
(define %paper-type% "A4")

</style-specification-body>
</style-specification>

<style-specification id="html" use="docbook">
<style-specification-body>

<!-- See docbook-dsssl-1.x/doc -->
(define %generate-reference-toc% #t)
(define %number-programlisting-lines% #f)
(define %section-autolabel% #t)
(define bop-footnotes #t)
<!-- (define %show-comments% #f) -->
(define ($generate-book-lot-list$)
  (list (normalize "figure")
    (normalize "example")))

</style-specification-body>
</style-specification>

<external-specification id="docbook" document="docbook.dsl">

</style-sheet>
