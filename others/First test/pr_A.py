voices = input().split()
cnt = 0

for i in range(4):
    if i == 0 and voices[i]:
        cnt += 2
	
    elif voices[i]:
        cnt += 1

if cnt > 2:
	print("YES")
else:
	print("NO")
