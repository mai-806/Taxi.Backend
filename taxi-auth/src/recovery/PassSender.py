import argparse
import smtplib
from email.mime.text import MIMEText

def get_args():
    parser = argparse.ArgumentParser(description='New Password Sender')
    parser.add_argument('-e', '--email', type=str, help='Input the recipient email')
    parser.add_argument('-p', '--password', type=str, help='Input the password')

    args = parser.parse_args()

    return (args.email, args.password)


def send_email(recipient_email, message):
    sender = "taximai2023@gmail.com"
    password = "vouifrnlcthizaau"

    server = smtplib.SMTP("smtp.gmail.com", 587)
    server.starttls()

    try:
        server.login(sender, password)
        msg = MIMEText(message)
        msg["Subject"] = "Password Recovery"
        server.sendmail(sender, recipient_email, msg.as_string())

        return "The message was sent successfully!"
    except Exception as _ex:
        return f"{_ex}\nCheck your login or password please!"
    

def main():
    recipient_email, password = get_args()
    message = "You requested password recovery in our taxi service.\nYour new password: " + password
    print(send_email(recipient_email=recipient_email, message=message))


if __name__ == "__main__":
    main()