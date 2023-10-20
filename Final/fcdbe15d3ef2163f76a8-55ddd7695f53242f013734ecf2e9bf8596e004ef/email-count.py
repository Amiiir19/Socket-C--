"""Script to count the emails received in a gmail account and posts it to a web hook.
"""
import imaplib
import optparse
import json
import urllib2

def count_emails(username, password, folder="INBOX"):
    """Returns the number of emails in specified folder.
    """
    client = imaplib.IMAP4_SSL('imap.gmail.com','993')
    client.login(username, password)
    count = client.select(folder)
    return count[1][0]

def main():
    p = optparse.OptionParser()
    p.add_option("--email", help="E-mail address of the gmail account to find the count")
    p.add_option("--password", help="Password of the gmail account")
    p.add_option("--password-file", help="Path to the file containing the password")
    p.add_option("--label", help="Name of the label/folder to find message count (default: %default)", default="INBOX")
    p.add_option("--post-hook", help="URL of the webhook to POST the count")

    options, args = p.parse_args()

    if not options.email:
        raise Exception("Please provide the email address to check the count")

    if not options.password and not options.password_file:
        raise Exception("Please provide either password or password-file.")

    username = options.email
    if options.password:
        pw = options.password
    else:
        pw = open(options.password_file).read().strip()

    count = count_emails(username, pw, options.label)
    print(count)
    if options.post_hook:
        jsontext = json.dumps({"email_count": count})
        req = urllib.request.Request(options.post_hook, jsontext, headers={"Content-Type": "application/json"})
        urllib.urlopen(req).read()

if __name__ == "__main__":
    main()