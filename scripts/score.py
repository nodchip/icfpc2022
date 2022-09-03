import json

def main():
    j = json.load(open('user.txt'))
    sum_min_cost = 0
    for result in j['results']:
        sum_min_cost += result['min_cost']
        print(result)
    print("Total score = " + str(sum_min_cost))

if __name__ == '__main__':
	main()
