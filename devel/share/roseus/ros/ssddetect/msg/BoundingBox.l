;; Auto-generated. Do not edit!


(when (boundp 'ssddetect::BoundingBox)
  (if (not (find-package "SSDDETECT"))
    (make-package "SSDDETECT"))
  (shadow 'BoundingBox (find-package "SSDDETECT")))
(unless (find-package "SSDDETECT::BOUNDINGBOX")
  (make-package "SSDDETECT::BOUNDINGBOX"))

(in-package "ROS")
;;//! \htmlinclude BoundingBox.msg.html


(defclass ssddetect::BoundingBox
  :super ros::object
  :slots (_Class _probability _xmin _ymin _width _height ))

(defmethod ssddetect::BoundingBox
  (:init
   (&key
    ((:Class __Class) "")
    ((:probability __probability) 0.0)
    ((:xmin __xmin) 0)
    ((:ymin __ymin) 0)
    ((:width __width) 0)
    ((:height __height) 0)
    )
   (send-super :init)
   (setq _Class (string __Class))
   (setq _probability (float __probability))
   (setq _xmin (round __xmin))
   (setq _ymin (round __ymin))
   (setq _width (round __width))
   (setq _height (round __height))
   self)
  (:Class
   (&optional __Class)
   (if __Class (setq _Class __Class)) _Class)
  (:probability
   (&optional __probability)
   (if __probability (setq _probability __probability)) _probability)
  (:xmin
   (&optional __xmin)
   (if __xmin (setq _xmin __xmin)) _xmin)
  (:ymin
   (&optional __ymin)
   (if __ymin (setq _ymin __ymin)) _ymin)
  (:width
   (&optional __width)
   (if __width (setq _width __width)) _width)
  (:height
   (&optional __height)
   (if __height (setq _height __height)) _height)
  (:serialization-length
   ()
   (+
    ;; string _Class
    4 (length _Class)
    ;; float64 _probability
    8
    ;; int64 _xmin
    8
    ;; int64 _ymin
    8
    ;; int64 _width
    8
    ;; int64 _height
    8
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; string _Class
       (write-long (length _Class) s) (princ _Class s)
     ;; float64 _probability
       (sys::poke _probability (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; int64 _xmin
#+(or :alpha :irix6 :x86_64)
       (progn (sys::poke _xmin (send s :buffer) (send s :count) :long) (incf (stream-count s) 8))
#-(or :alpha :irix6 :x86_64)
       (cond ((and (class _xmin) (= (length (_xmin . bv)) 2)) ;; bignum
              (write-long (ash (elt (_xmin . bv) 0) 0) s)
              (write-long (ash (elt (_xmin . bv) 1) -1) s))
             ((and (class _xmin) (= (length (_xmin . bv)) 1)) ;; big1
              (write-long (elt (_xmin . bv) 0) s)
              (write-long (if (>= _xmin 0) 0 #xffffffff) s))
             (t                                         ;; integer
              (write-long _xmin s)(write-long (if (>= _xmin 0) 0 #xffffffff) s)))
     ;; int64 _ymin
#+(or :alpha :irix6 :x86_64)
       (progn (sys::poke _ymin (send s :buffer) (send s :count) :long) (incf (stream-count s) 8))
#-(or :alpha :irix6 :x86_64)
       (cond ((and (class _ymin) (= (length (_ymin . bv)) 2)) ;; bignum
              (write-long (ash (elt (_ymin . bv) 0) 0) s)
              (write-long (ash (elt (_ymin . bv) 1) -1) s))
             ((and (class _ymin) (= (length (_ymin . bv)) 1)) ;; big1
              (write-long (elt (_ymin . bv) 0) s)
              (write-long (if (>= _ymin 0) 0 #xffffffff) s))
             (t                                         ;; integer
              (write-long _ymin s)(write-long (if (>= _ymin 0) 0 #xffffffff) s)))
     ;; int64 _width
#+(or :alpha :irix6 :x86_64)
       (progn (sys::poke _width (send s :buffer) (send s :count) :long) (incf (stream-count s) 8))
#-(or :alpha :irix6 :x86_64)
       (cond ((and (class _width) (= (length (_width . bv)) 2)) ;; bignum
              (write-long (ash (elt (_width . bv) 0) 0) s)
              (write-long (ash (elt (_width . bv) 1) -1) s))
             ((and (class _width) (= (length (_width . bv)) 1)) ;; big1
              (write-long (elt (_width . bv) 0) s)
              (write-long (if (>= _width 0) 0 #xffffffff) s))
             (t                                         ;; integer
              (write-long _width s)(write-long (if (>= _width 0) 0 #xffffffff) s)))
     ;; int64 _height
#+(or :alpha :irix6 :x86_64)
       (progn (sys::poke _height (send s :buffer) (send s :count) :long) (incf (stream-count s) 8))
#-(or :alpha :irix6 :x86_64)
       (cond ((and (class _height) (= (length (_height . bv)) 2)) ;; bignum
              (write-long (ash (elt (_height . bv) 0) 0) s)
              (write-long (ash (elt (_height . bv) 1) -1) s))
             ((and (class _height) (= (length (_height . bv)) 1)) ;; big1
              (write-long (elt (_height . bv) 0) s)
              (write-long (if (>= _height 0) 0 #xffffffff) s))
             (t                                         ;; integer
              (write-long _height s)(write-long (if (>= _height 0) 0 #xffffffff) s)))
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; string _Class
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _Class (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;; float64 _probability
     (setq _probability (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; int64 _xmin
#+(or :alpha :irix6 :x86_64)
      (setf _xmin (prog1 (sys::peek buf ptr- :long) (incf ptr- 8)))
#-(or :alpha :irix6 :x86_64)
      (setf _xmin (let ((b0 (prog1 (sys::peek buf ptr- :integer) (incf ptr- 4)))
                  (b1 (prog1 (sys::peek buf ptr- :integer) (incf ptr- 4))))
              (cond ((= b1 -1) b0)
                     ((and (= b1  0)
                           (<= lisp::most-negative-fixnum b0 lisp::most-positive-fixnum))
                      b0)
                    ((= b1  0) (make-instance bignum :size 1 :bv (integer-vector b0)))
                    (t (make-instance bignum :size 2 :bv (integer-vector b0 (ash b1 1)))))))
   ;; int64 _ymin
#+(or :alpha :irix6 :x86_64)
      (setf _ymin (prog1 (sys::peek buf ptr- :long) (incf ptr- 8)))
#-(or :alpha :irix6 :x86_64)
      (setf _ymin (let ((b0 (prog1 (sys::peek buf ptr- :integer) (incf ptr- 4)))
                  (b1 (prog1 (sys::peek buf ptr- :integer) (incf ptr- 4))))
              (cond ((= b1 -1) b0)
                     ((and (= b1  0)
                           (<= lisp::most-negative-fixnum b0 lisp::most-positive-fixnum))
                      b0)
                    ((= b1  0) (make-instance bignum :size 1 :bv (integer-vector b0)))
                    (t (make-instance bignum :size 2 :bv (integer-vector b0 (ash b1 1)))))))
   ;; int64 _width
#+(or :alpha :irix6 :x86_64)
      (setf _width (prog1 (sys::peek buf ptr- :long) (incf ptr- 8)))
#-(or :alpha :irix6 :x86_64)
      (setf _width (let ((b0 (prog1 (sys::peek buf ptr- :integer) (incf ptr- 4)))
                  (b1 (prog1 (sys::peek buf ptr- :integer) (incf ptr- 4))))
              (cond ((= b1 -1) b0)
                     ((and (= b1  0)
                           (<= lisp::most-negative-fixnum b0 lisp::most-positive-fixnum))
                      b0)
                    ((= b1  0) (make-instance bignum :size 1 :bv (integer-vector b0)))
                    (t (make-instance bignum :size 2 :bv (integer-vector b0 (ash b1 1)))))))
   ;; int64 _height
#+(or :alpha :irix6 :x86_64)
      (setf _height (prog1 (sys::peek buf ptr- :long) (incf ptr- 8)))
#-(or :alpha :irix6 :x86_64)
      (setf _height (let ((b0 (prog1 (sys::peek buf ptr- :integer) (incf ptr- 4)))
                  (b1 (prog1 (sys::peek buf ptr- :integer) (incf ptr- 4))))
              (cond ((= b1 -1) b0)
                     ((and (= b1  0)
                           (<= lisp::most-negative-fixnum b0 lisp::most-positive-fixnum))
                      b0)
                    ((= b1  0) (make-instance bignum :size 1 :bv (integer-vector b0)))
                    (t (make-instance bignum :size 2 :bv (integer-vector b0 (ash b1 1)))))))
   ;;
   self)
  )

(setf (get ssddetect::BoundingBox :md5sum-) "903accce26e7f49eac1a268718f1fd1e")
(setf (get ssddetect::BoundingBox :datatype-) "ssddetect/BoundingBox")
(setf (get ssddetect::BoundingBox :definition-)
      "string Class
float64 probability
int64 xmin
int64 ymin
int64 width
int64 height


")



(provide :ssddetect/BoundingBox "903accce26e7f49eac1a268718f1fd1e")


