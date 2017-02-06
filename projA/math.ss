(define zero?
    (lambda (x)
      (cond
	((equal? 0 x) #t)
	(else #f))))

(define positive?
(lambda (x)
    ( if (>= x 0)
    #t
    #f)))
    
(define negative?
  (lambda (x)
    ( if (>= x 0)
         #f
         #t)))
         
(define square
  (lambda (x)
    (* x x)))
    
(define abs
  (lambda (x)
    (cond ((< x 0) (* -1 x))
          ((> x 0) x))))