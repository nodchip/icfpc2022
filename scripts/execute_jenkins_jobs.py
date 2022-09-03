import argparse
import requests
import urllib.parse

def main():
    parser = argparse.ArgumentParser(description='Brain Wall batch solver')
    parser.add_argument(
        '--user_name', help='Jenkins User Name.', required=True)
    parser.add_argument('--token', help='Jenkins API Token.', required=True)
    parser.add_argument(
        '--url', help='Jenkins Project URL. ex) http://jenkins:8080/job/ICFPC2021.optimize_parameters.2021-07-11/', required=True)

    parser.add_argument('--stop', action='store_true',
                        help='Stop the current jobs.')
    parser.add_argument('--first_build_number', type=int,
                        help='First build number to be stopped.')
    parser.add_argument('--last_build_number', type=int,
                        help='Last build number to be stopped.')

    parser.add_argument('--start', action='store_true', help='Start jobs.')
    parser.add_argument('--solver_name', type=str, help='Solver name')
    parser.add_argument('--first_problem_number', type=int,
                        help='First problem number to be started.')
    parser.add_argument('--last_problem_number', type=int,
                        help='Last problem number to be started.')
    parser.add_argument('--interval_dp_num_intervals', type=int,
                        help='Interval DP num intervals.')
    parser.add_argument('--interval_dp_2_num_intervals', type=int,
                        help='Interval DP2 num intervals.')
    parser.add_argument('--interval_dp_3_num_intervals', type=int,
                        help='Interval DP3 num intervals.')

    args = parser.parse_args()

    if args.stop:
        for build_number in range(args.first_build_number, args.last_build_number + 1):
            url = f'{args.url}{build_number}/stop'
            print(url)
            response = requests.post(url, auth=('hnoda', args.token))
            print(response.status_code)

    if args.start:
        for problem_number in range(args.first_problem_number, args.last_problem_number + 1):
            # Parameterized Build - Jenkins - Jenkins Wiki https://wiki.jenkins.io/display/JENKINS/Parameterized+Build
            parameters = {
                'INTERNAL_DP_NUM_INTERVALS': str(args.interval_dp_num_intervals),
                'INTERNAL_DP_2_NUM_INTERVALS': str(args.interval_dp_2_num_intervals),
                'INTERNAL_DP_3_NUM_INTERVALS': str(args.interval_dp_3_num_intervals),
                'PROBLEM_NUMBER': str(problem_number),
                'SOLVER_NAME': str(args.solver_name),
            }
            url = f'{args.url}buildWithParameters?{urllib.parse.urlencode(parameters)}'
            print(url)
            requests.post(url, auth=(args.user_name, args.token))


if __name__ == '__main__':
    main()
