g++ main.cpp -o main.o -lmysqlcppconn

CREATE USER 'admin'@'localhost' IDENTIFIED BY 'admin';
GRANT ALL PRIVILEGES ON lista5.* TO 'admin'@'localhost';
FLUSH PRIVILEGES;

CREATE USER 'user'@'localhost' IDENTIFIED BY 'user';
GRANT ALL PRIVILEGES ON lista5.* TO 'admin'@'localhost';
FLUSH PRIVILEGES;


CREATE TABLE User (
    id INT AUTO_INCREMENT PRIMARY KEY,
    nick VARCHAR(100),
    firstName VARCHAR(100),
    lastName VARCHAR(100),
    email VARCHAR(255) UNIQUE,
    password VARCHAR(255) NOT NULL
);

CREATE TABLE Author (
    id INT AUTO_INCREMENT PRIMARY KEY,
    firstName VARCHAR(100),
    lastName VARCHAR(100)
);

CREATE TABLE Category (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(100)
);

CREATE TABLE Book (
    id INT AUTO_INCREMENT PRIMARY KEY,
    title VARCHAR(255) NOT NULL,
    authorId INT,
    isbn VARCHAR(20) NOT NULL,
    year INT,
    CHECK (isbn REGEXP '^[0-9]{13}$'),
    FOREIGN KEY (authorId) REFERENCES Author(id)
);

CREATE TABLE Borrow (
    id INT AUTO_INCREMENT PRIMARY KEY,
    userId INT,
    bookId INT UNIQUE,
    borrowDate DATE,
    returnDate DATE,
    FOREIGN KEY (userId) REFERENCES User(id),
    FOREIGN KEY (bookId) REFERENCES Book(id)
);

CREATE TABLE Category_Book (
    bookId INT,
    categoryId INT,
    FOREIGN KEY (bookId) REFERENCES Book(id),
    FOREIGN KEY (categoryId) REFERENCES Category(id)
);


DELIMITER $$

CREATE TRIGGER checkIsbn
BEFORE INSERT ON Book
FOR EACH ROW
BEGIN
    DECLARE number INT;
    
    SELECT COUNT(*)
    INTO number
    FROM Book
    WHERE NEW.isbn=Book.isbn AND NEW.title!=Book.title;


    IF number > 0 THEN
        SIGNAL SQLSTATE '45000'
        SET MESSAGE_TEXT = 'Isbn already exists';
    END IF;
END $$

DELIMITER ;

DELIMITER $$

CREATE TRIGGER checkYear
BEFORE INSERT ON Book
FOR EACH ROW
BEGIN
    IF NEW.year > YEAR(CURDATE()) THEN
        SIGNAL SQLSTATE '45000'
        SET MESSAGE_TEXT = 'Release date cannot be in the past';
    END IF;
END $$

DELIMITER ;

//procedura

DELIMITER $$

CREATE PROCEDURE AddBook(
    IN _title VARCHAR(255),
    IN _authorFName VARCHAR(255),
    IN _authorLName VARCHAR(255),
    IN _year INT,
    IN _isbn VARCHAR(20)
)
BEGIN
    DECLARE number INT;

    SELECT COUNT(*)
    INTO number
    FROM Author
    WHERE firstName=_authorFName AND lastName=_authorLName;

    IF number = 0 THEN
        INSERT INTO Author (firstName, lastName)
        VALUES (_authorFName, _authorLName);
    END IF;

    SELECT id
    INTO number
    FROM Author
    WHERE firstName=_authorFName AND lastName=_authorLName
    LIMIT 1;

    INSERT INTO Book (title, authorID, isbn, year)
    VALUES (_title, number, _isbn, _year);
END $$

DELIMITER ;


call AddBook("bookTitle","authorF","authorL",2137,"abcdefg");
call AddBook("bookTitle","authorF","authorL",1000,"1111111111111");

//procedura 2

DELIMITER $$

CREATE PROCEDURE UpdateBook(
    IN _bookId INT,
    IN _title VARCHAR(255),
    IN _authorFName VARCHAR(255),
    IN _authorLName VARCHAR(255),
    IN _year INT,
    IN _isbn VARCHAR(20)
)
BEGIN
    DECLARE authorId INT;
    DECLARE number INT;

    SELECT COUNT(*)
    INTO number
    FROM Author
    WHERE firstName = _authorFName AND lastName = _authorLName;

    IF number = 0 THEN
        INSERT INTO Author (firstName, lastName)
        VALUES (_authorFName, _authorLName);
    END IF;

    SELECT id
    INTO authorId
    FROM Author
    WHERE firstName = _authorFName AND lastName = _authorLName
    LIMIT 1;

    UPDATE Book
    SET title = _title,
        authorId = authorId,
        isbn = _isbn,
        year = _year
    WHERE id = _bookId;

END $$

DELIMITER ;


//trigery do autorów

DELIMITER $$

CREATE TRIGGER delAuthort
AFTER DELETE ON Book
FOR EACH ROW
BEGIN
    DECLARE number INT;
    SELECT COUNT(*) INTO number
    FROM Author
    WHERE Author.id=OLD.authorId;

    IF number=0 THEN
        DELETE FROM Author
        WHERE Author.id=OLD.authorId;
    END IF;
END $$

DELIMITER ;


DELIMITER $$

CREATE TRIGGER delAuthor
AFTER UPDATE ON Book
FOR EACH ROW
BEGIN
    DECLARE number INT;
    SELECT COUNT(*) INTO number
    FROM Author
    WHERE Author.id=OLD.authorId;

    IF number=0 THEN
        DELETE FROM Author
        WHERE Author.id=OLD.authorId;
    END IF;
END $$

DELIMITER ;


//user valid

DELIMITER $$

CREATE TRIGGER validateUser
BEFORE INSERT ON User
FOR EACH ROW
BEGIN
    IF NEW.email NOT REGEXP '^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$' THEN
        SIGNAL SQLSTATE '45000'
        SET MESSAGE_TEXT = 'Invalid email format';
    END IF;

    IF NEW.password NOT REGEXP '[0-9]' THEN
        SIGNAL SQLSTATE '45000'
        SET MESSAGE_TEXT = 'Password must contain at least one digit';
    END IF;

    IF NEW.password NOT REGEXP '[!@#$%^&*(),.?":{}|<>]' THEN
        SIGNAL SQLSTATE '45000'
        SET MESSAGE_TEXT = 'Password must contain at least one special character';
    END IF;
END$$

DELIMITER ;

//autoryzacja

DELIMITER $$

CREATE FUNCTION userAuth(
    user VARCHAR(255),
    pass VARCHAR(255)
) RETURNS BOOLEAN
READS SQL DATA
BEGIN
    DECLARE number INT;

    SELECT COUNT(*) INTO number FROM User
    WHERE User.password = pass AND User.nick = user;

    IF number > 0 THEN
        RETURN TRUE;
    ELSE
        RETURN FALSE;
    END IF;
END $$

DELIMITER ;

GRANT EXECUTE ON PROCEDURE userSearch TO 'user'@'localhost';
GRANT EXECUTE ON FUNCTION userAuth TO 'user'@'localhost';   


drop procedure userSearch;

DELIMITER $$

CREATE PROCEDURE userSearch (
    IN p_title VARCHAR(255),
    IN p_firstName VARCHAR(255),
    IN p_lastName VARCHAR(255),
    IN p_isbn VARCHAR(20),
    IN p_year VARCHAR(4),
    IN p_genre VARCHAR(100)
)
BEGIN
    SELECT DISTINCT Book.id, Book.title, Book.year
    FROM Book
    INNER JOIN Author ON Author.id = Book.authorId
    LEFT JOIN Borrow ON Book.id = Borrow.bookId
    LEFT JOIN Category_Book ON Book.id=Category_Book.bookId
    LEFT JOIN Category ON Category_Book.categoryId=Category.id
    WHERE Borrow.userId IS NULL
    AND Book.title LIKE CONCAT('%', p_title, '%')
    AND Author.firstName LIKE CONCAT('%', p_firstName, '%')
    AND Author.lastName LIKE CONCAT('%', p_lastName, '%')
    AND Book.isbn LIKE CONCAT('%', p_isbn, '%')
    AND Book.year LIKE CONCAT('%', p_year, '%')
    AND (Category.id IS NULL OR Category.name LIKE CONCAT('%', p_genre, '%'));
END$$

DELIMITER ;

GRANT EXECUTE ON PROCEDURE userSearch TO 'user'@'localhost';

