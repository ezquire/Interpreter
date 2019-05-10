print "Testing recursive function calls, array.pop(), len(array)"
print "and array subscription."

def x(a,b,c,d):
  if len(a) == 0:
    return "good"
  if a[len(a) -1 ] == b or  d-3 == (d - 3 + a[0]):
    a.pop()
    x(a, b,c,d)
  if len(a) > 0 and a[0] > 0:
      a[0] = a[0] - 1
  return x(a, b,c-1,d-1)

a = [4,2,3,1,4]
c = 2
b = a[3]*c
d = 10
e = x(a,b,c,d)
print e
print "If the test passed the previous line should be the word good"

print " "

print "Testing array init, array.append() and printing arrays"
c = []
c.append("hello")
print "c should now be a string array"
print c
print "If the test passed the  previous line should be ['hello']"
